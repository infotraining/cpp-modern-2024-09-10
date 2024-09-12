#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <iostream>
#include <optional>
#include <string_view>
#include <type_traits>

using namespace std::literals;

// Modernize the following code using move semantics and/or perfect forwarding
class Player
{
public:
    // Player(int id, const std::string& name, const std::vector<int>& scores)
    //     : id_(id)
    //     , name_(name)
    //     , scores_(scores)
    // {
    // }

    Player(int id, std::string name, std::vector<int> scores)
        : id_{id}
        , name_{std::move(name)}
        , scores_{std::move(scores)}
    { }

    const int id() const { return id_; }
    const std::string& name() const { return name_; }
    const std::vector<int>& scores() const { return scores_; }

private:
    int id_;
    std::string name_;
    std::vector<int> scores_;
};

TEST_CASE("Modernizing Player class")
{
    Player p1(1, "John", {1, 2, 3, 4, 5});

    std::string player_name = "John Junior Unknown The Third";
    std::vector<int> player_scores = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Player p2(2, player_name, player_scores);
}

///////////////////////////////////////////////////////////////////////////////
// Why this function is buggy?

template <typename T>
auto make_vector(T&& obj)
{    
    using TValue = std::remove_cvref_t<T>;
    std::vector<TValue> vec;

    vec.push_back(std::forward<T>(obj));

    //...

    return vec;
}

TEST_CASE("Bug#1")
{
    SECTION("lvalue")
    {
        std::string text = "Text.......................";

        auto vec = make_vector(text);
        REQUIRE(vec.front() == "Text.......................");

        REQUIRE(text == "Text.......................");
    }

    SECTION("rvalue")
    {        
        auto vec = make_vector(std::string("Text......................."));
        REQUIRE(vec.front() == "Text.......................");

        std::string text = "Text.......................";
        auto vec2 = make_vector(std::move(text));
    }
}

///////////////////////////////////////////////////////////////////////////////
struct Value
{
    Value() { std::cout << "Default ctor\n"; }
    ~Value() { std::cout << "Dtor\n"; }
    Value(const Value& other) { std::cout << "Copy ctor\n"; }
    Value(Value&& other) { std::cout << "Move ctor\n"; }
    Value& operator=(const Value& other)
    {
        std::cout << "Copy assignment\n";
        return *this;
    }
    Value& operator=(Value&& other)
    {
        std::cout << "Move assignment\n";
        return *this;
    }
};

template <typename T>
struct Data
{
    T data;

    static_assert(!std::is_reference_v<T>, "T cannot be a reference type");

    // void push(const T& value)
    // {
    //     data = value;
    // }

    // void push(T&& value)
    // {
    //     data = std::move(value);
    // }

    template <typename U>
    void push(U&& value)
    {
        data = std::forward<U>(value);
    }
};

TEST_CASE("Bug#2")
{
    SECTION("what will happen? - 1")
    {
        Data<Value> data;

        Value value;
        data.push(std::move(value));
    }

    SECTION("what will happen? - 2")
    {
        Data<Value> data;
        Value value;

        data.push(value); // uncomment the line - why this does not compile?
    }

    SECTION("Data<string>")
    {
        Data<std::string> data;

        data.push("text"s);
        data.push("text");

        //Data<std::string&> data2;
    }
}
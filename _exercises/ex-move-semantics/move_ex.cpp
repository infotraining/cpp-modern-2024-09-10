#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <iostream>
#include <optional>
#include <string_view>

// Modernize the following code using move semantics and/or perfect forwarding
class Player
{
public:
    Player(int id, const std::string& name, const std::vector<int>& scores)
        : id_(id)
        , name_(name)
        , scores_(scores)
    {
    }

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

    vec.push_back(std::move(obj));

    //...

    return vec;
}

TEST_CASE("Bug#1")
{
    std::string text = "Text.......................";

    auto vec = make_vector(text);
    REQUIRE(vec.front() == "Text.......................");

    // REQUIRE(text == "Text.......................");
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
    
    void push(T&& value)
    {
        data = std::forward<T>(value);
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

        // data.push(value); // uncomment the line - why this does not compile?
    }
}
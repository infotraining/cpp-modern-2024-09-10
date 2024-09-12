#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std::literals;

template <typename T>
concept StlRange = requires(T c) {
    { c.begin() } -> std::input_or_output_iterator;
    { c.end() } -> std::input_or_output_iterator;
};

template <typename T>
concept PrintableStlRange = StlRange<T> && requires(std::ranges::range_value_t<T> obj) {
    std::cout << obj;
};

template<PrintableStlRange T>
void print(T&& c, std::string_view prefix = "items")
{
    std::cout << prefix << ": [ ";
    for (const auto& item : c)
        std::cout << item << " ";
    std::cout << "]\n";
}

TEST_CASE("print")
{
    std::vector vec = { "abc", "def", "ghi" };
    print(vec, "vec");

    // std::map<int, std::string> dict = {{1, "one"}};
    // print(dict, "dict");
}

// template <typename T>
// concept Pointer = std::is_pointer_v<T>;

template <typename T>
concept Pointer = std::is_pointer_v<T> || requires(T ptr) {
    *ptr;
    ptr == nullptr;
    ptr != nullptr;
};

template <typename T>
concept Indexable = requires(T obj, size_t index) {
    obj[index];
};

static_assert(Indexable<std::vector<int>>);
static_assert(!Indexable<std::list<int>>);

template <typename T>
concept LeanType = sizeof(T) < sizeof(int);

namespace ver_1
{
    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <typename T>
        requires std::is_pointer_v<T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return max_value(*a, *b);
    }
} // namespace ver_1

namespace ver_2
{
    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <Pointer T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return max_value(*a, *b);
    }
} // namespace ver_2

inline namespace ver_3
{
    inline namespace ver_2
    {
        auto max_value(auto a, auto b)
        {
            return a < b ? b : a;
        }

        auto max_value(Pointer auto a, Pointer auto b) // Pointer<decltype(a)> && Pointer<decltype(b)>
        {
            assert(a != nullptr);
            assert(b != nullptr);

            return max_value(*a, *b);
        }
    } // namespace ver_2
} // namespace ver_3

TEST_CASE("constraints", "[concepts]")
{
    int x = 10;
    int y = 20;

    CHECK(max_value(x, y) == 20);

    CHECK(max_value(&x, &y) == 20);

    auto sp1 = std::make_shared<int>(10);
    auto sp2 = std::make_shared<int>(20);
    CHECK(max_value(sp1, sp2) == 20);
}

template <LeanType T>
void use(T obj)
{
    std::cout << "Using lean type...\n";
}

TEST_CASE("concepts")
{
    use(true);
    // use(std::string{});
}

std::unsigned_integral auto id()
{
    static size_t i = 0;
    return ++i;
}

TEST_CASE("auto + concepts")
{
    std::unsigned_integral auto id_value1 = id();   // std::unsigned_integral<decltype(id_value1)>
    std::convertible_to<size_t> auto value2 = id(); // std::convertible_to<decltype(value), size_t>
}

template <typename T>
    requires                 // requires clause
    requires(T a) { a + a; } // requires expression
auto add(T a, T b)
{
    return a + b;
}

template <typename TContainer, typename TValue>
void add_to_container(TContainer& container, TValue&& value)
{
    if constexpr (requires { container.push_back(std::forward<TValue>(value)); })
    {
        container.push_back(std::forward<TValue>(value));
    }
    else
    {
        container.insert(std::forward<TValue>(value));
    }
}

TEST_CASE("add")
{
    REQUIRE(add(50, 34) == 84);
    // REQUIRE(add(std::vector{1, 2, 3}, std::vector{4}));

    std::vector<int> vec;
    add_to_container(vec, 42);

    std::set<int> mset;
    add_to_container(mset, 42);

    auto lambda = [](auto item) {
        using T = int;
        constexpr bool test = requires(T obj) { obj[0]; };

        return test;
    };

    constexpr auto result = lambda(42);
}

template <typename T>
concept FaultyLeanType = requires {
    requires sizeof(T) < 8;
    requires std::is_integral_v<T>;
};

template <typename T>
concept Sizeable = requires(T obj) {
    { obj.size() } -> std::convertible_to<size_t>;
};

TEST_CASE("FaultyLeanType")
{
    static_assert(!FaultyLeanType<std::vector<int>>);
    static_assert(FaultyLeanType<char>);
}
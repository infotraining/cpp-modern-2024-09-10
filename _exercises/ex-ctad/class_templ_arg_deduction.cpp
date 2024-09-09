#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std::literals;

template <typename T>
struct Range
{
    T low, high;
};

// TODO - write deduction guides

TEST_CASE("CTAD for Range")
{
    // Range r1{4, 5};
    // static_assert(std::is_same_v<decltype(r1), Range<int>>);

    // Range r2{3.14, 6.28};
    // static_assert(std::is_same_v<decltype(r2), Range<double>>);

    // SECTION("extra")
    // {
    //     Range r3{1, 3.14};
    //     static_assert(std::is_same_v<decltype(r3), Range<double>>);
    // }
}

////////////////////////////////////////////

template <typename T>
struct Wrapper
{
    T value;

    explicit Wrapper(T val)
        : value(std::move(val))
    {
    }
};

// TODO - write deduction guides

TEST_CASE("CTAD for Wrapper")
{
    // std::string s = "text";

    // Wrapper w1{s};
    // static_assert(std::is_same_v<decltype(w1), Wrapper<std::string>>);

    // Wrapper w2{42};
    // static_assert(std::is_same_v<decltype(w2), Wrapper<int>>);

    // Wrapper w3{"abc"};
    // static_assert(std::is_same_v<decltype(w3), Wrapper<std::string>>);
    // REQUIRE(w3.value == "abc"s);
}

///////////////////////////////////////
// EXTRA

template <typename T, size_t N>
struct Array
{
    T items[N];
};

// TODO - write deduction guides

// TEST_CASE("CTAD for Array")
// {
//     Array arr1{1, 2, 3};
//     static_assert(std::is_same_v<decltype(arr1), Array<int, 3>>);

//     Array arr2{"abc", "def", "ghi", "klm"};
//     static_assert(std::is_same_v<decltype(arr2), Array<const char*, 4>>);

//     SECTION("extra")
//     {
//         Array arr3{1.0, 2.3, 3.1, 4.0, 5.0}; // it should be an error - all items on the list should have the same type
//     }
// }
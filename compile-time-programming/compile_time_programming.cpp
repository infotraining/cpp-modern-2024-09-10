#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <array>
#include <algorithm>
#include <ranges>

using namespace std::literals;

template <size_t N>
constexpr auto create_lookup_table()
{
    std::array<uint32_t, N> squares{};
    std::vector<int> vec_squares;
    vec_squares.reserve(N);

    auto squares_view = std::views::iota(1) 
        | std::views::transform([](int a) { return a * a; }) 
        | std::views::take(N);

    std::ranges::copy(squares_view, std::back_inserter(vec_squares));
    std::ranges::copy(vec_squares, squares.begin());

    //vec_squares[1024] = 665;

    // int* ptr = nullptr;
    // *ptr = 665;

    return squares;
}

TEST_CASE("lookup table")
{
    constexpr auto squares_lookup_table = create_lookup_table<100>();
}

consteval int cube(int x)
{
    return x * x * x;
}

TEST_CASE("consteval")
{
    int c = cube(11);

    std::vector cubes = { cube(9), cube(11), cube(55) };
}

TEST_CASE("constexpr extensions")
{
    REQUIRE(true);
}
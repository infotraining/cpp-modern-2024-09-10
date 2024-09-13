#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <ranges>
#include <span>
#include <string>
#include <vector>
#include <numbers>

using namespace std::literals;

void print(std::span<const int> data, std::string_view desc)
{
    std::cout << desc << ": [ ";
    for (const auto& item : data)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

void zero(std::span<int> data, int value = 0)
{
    std::ranges::fill(data, value);
}

TEST_CASE("span")
{
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    SECTION("passing to function")
    {
        print(vec, "vec");
    }

    SECTION("fixed extent")
    {
        std::span<int, 5> slice_1{vec.data(), 5};
        print(slice_1, "slice_1");
    }

    SECTION("dynamic extent")
    {
        std::span<int> slice_2{vec.data() + 2, 5};
        print(slice_2, "slice_2");
    }

    SECTION("native array")
    {
        int tab[10] = {34, 2345, 623, 345, 53, 65, 234, 345};
        print(tab, "tab");
    }

    SECTION("reference semantics")
    {
        std::span first_half{vec.data(), vec.size() / 2};
        zero(first_half);
        print(vec, "vec");

        std::ranges::sort(std::span(vec.begin() + vec.size() / 2, vec.end()), std::greater{});
        print(vec, "vec");

        int* buffer = new int[1024];
        size_t size = 1024;

        std::span buffer_span{buffer, size};
        zero(buffer_span);
    }
}

TEST_CASE("std::span - subspan")
{
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 0);

    print(vec, "vec");

    const size_t col_size = 10;

    for (size_t row = 0; row < vec.size() / col_size; ++row)
    {
        auto row_data = std::span{vec}.subspan(row * col_size, col_size);

        print(row_data, std::format("Row#{}", row));
    }
}

TEST_CASE("span for string")
{
    std::string str = "abc def";

    std::span token{str.data(), 3};

    std::ranges::transform(token, token.begin(), [](auto c) { return std::toupper(c); });

    REQUIRE(str == "ABC def");
}

TEST_CASE("span read-only")
{
    const std::vector<int> vec = {1, 2, 3, 4};

    std::span slice{vec};
}

TEST_CASE("std::byte")
{
    SECTION("legacy")
    {
        char byte1 = 0b0000'1111;
        char byte2 = 0b0100'1111;

        char byte3 = ~(byte1 | byte2);

        //byte1 + byte2;
        std::cout << byte3 << "\n";
    }

    SECTION("std::byte")
    {
        std::byte byte1{0b0000'1111};
        std::byte byte2{42};

        //byte1 + byte2;

        std::byte byte3 = ~(byte1 | byte2);

        std::cout << std::to_integer<int>(byte3) << "\n";
    }
}

void print_as_bytes(const float f, const std::span<const std::byte> bytes)
{
#ifdef __cpp_lib_format
    std::cout << std::format("{:+6}", f) << " - { ";

    for (std::byte b : bytes)
    {
        std::cout << std::format("{:02X} ", std::to_integer<int>(b));
    }

    std::cout << "}\n";
#endif
}

TEST_CASE("float as span of bytes")
{
    float data[] = {std::numbers::pi_v<float>};

    std::span<const std::byte> const_bytes = std::as_bytes(std::span{data});
    print_as_bytes(data[0], const_bytes);

    std::span<std::byte> writeable_bytes = std::as_writable_bytes(std::span{data});
    writeable_bytes[3] |= std::byte{0b1000'0000};
    print_as_bytes(data[0], const_bytes);
}
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cmath>

using namespace std::literals;

bool is_large(int x)
{
    return x > 8;
}

template <typename T>
struct IsLarge
{
    constexpr static bool value = sizeof(T) > 8;
};

// template variable
template <typename T>
constexpr bool IsLarge_v = IsLarge<T>::value;

TEST_CASE("type traits")
{
    static_assert(IsLarge<std::string>::value);
    static_assert(!IsLarge<bool>::value);
    static_assert(IsLarge_v<std::vector<int>>);
}

template <typename T>
struct Identity
{
    using type = T;
};

template <typename T>
using Identity_t = typename Identity<T>::type;

///////////////////////////////////////////////////

template <typename T1, typename T2>
struct IsSame
{
    constexpr static bool value = false;
};

template <typename T>
struct IsSame<T, T>
{
    constexpr static bool value = true;
};

template <typename T1, typename T2>
constexpr bool IsSame_v = IsSame<T1, T2>::value;

///////////////////////////////////////////////////

TEST_CASE("Identity")
{
    Identity<int>::type x;

    static_assert(IsSame<Identity_t<int>, int>::value);
    static_assert(IsSame_v<int, int>);
    static_assert(!IsSame_v<int, const int>);
}

///////////////////////////////////////////////////

template <typename T>
struct RemoveRef
{
    using type = T;
};

template <typename T>
struct RemoveRef<T&>
{
    using type = T;
};

template <typename T>
struct RemoveRef<T&&>
{
    using type = T;
};

template <typename T>
using RemoveRef_t = typename RemoveRef<T>::type;

TEST_CASE("RemoveRef")
{
    static_assert(IsSame_v<RemoveRef<int>::type, int>);

    using T1 = int&;
    using T2 = RemoveRef<T1>::type;

    static_assert(IsSame_v<T2, int>);
    static_assert(IsSame_v<RemoveRef_t<int&&>, int>);
}

/////////////////////////////////////////////////

template <typename T>
struct IsPointer : std::false_type
{
};

template <typename T>
struct IsPointer<T*> : std::true_type
{
};

template <typename T>
constexpr bool IsPointer_v = IsPointer<T>::value;

TEST_CASE("IsPointer")
{
    static_assert(IsPointer<int*>::value);
    static_assert(IsPointer<const int*>::value);
    static_assert(!IsPointer_v<const int>);
}

namespace BeforeCpp20
{
    template <typename T>
    auto is_power_of_2(T value) -> std::enable_if_t<std::is_integral_v<T>, bool>
    {
        return (value > 0) && (value & (value - 1)) == 0;
    }

    template <typename T>
    auto is_power_of_2(T value) -> std::enable_if_t<std::is_floating_point_v<T>, bool>
    {
        int exponent;
        T mantissa = std::frexp(value, &exponent);
        return mantissa == static_cast<T>(0.5);
    }
} // namespace Before Cpp20

bool is_power_of_2(std::integral auto value)
{
    return (value > 0) && (value & (value - 1)) == 0;
}

bool is_power_of_2(std::floating_point auto value) 
{
    int exponent;
    std::floating_point auto mantissa = std::frexp(value, &exponent);
    return mantissa == static_cast<decltype(value)>(0.5);
}

TEST_CASE("is_power_of_2")
{
    REQUIRE(is_power_of_2(1));
    REQUIRE(is_power_of_2(2));
    REQUIRE(is_power_of_2(4));
    REQUIRE(is_power_of_2(8));
    REQUIRE(is_power_of_2(16));
    REQUIRE(is_power_of_2(32));
    REQUIRE(is_power_of_2(64));

    REQUIRE(!is_power_of_2(9));

    REQUIRE(is_power_of_2(4.0));
    REQUIRE(is_power_of_2(4.0f));

    int x = -1;
    size_t y = 64;

    REQUIRE(std::cmp_less(x, y));
}
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

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

template<typename T1, typename T2>
constexpr bool IsSame_v = IsSame<T1, T2>::value;

///////////////////////////////////////////////////

TEST_CASE("Identity")
{
    Identity<int>::type x;

    static_assert(IsSame<Identity_t<int>, int>::value);
    static_assert(IsSame_v<int, int>);
    static_assert(!IsSame_v<int, const int>);
}
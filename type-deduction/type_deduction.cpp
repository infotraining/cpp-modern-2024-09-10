#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <map>
#include <catch2/catch_test_macros.hpp>


using namespace std;
using namespace std::literals;

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

void foo(int)
{ }

template <typename T>
void deduce1(T arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template <typename T>
void deduce2(T& arg)  // deduce2(auto& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template <typename T>
void deduce3(T&& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

TEST_CASE("type deduction rules")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];

    SECTION("Case 1")
    {
        deduce1(x);      // T = int; int arg;
        deduce1(cx);     // T = int; int arg; // removes const    
        deduce1(ref_x);  // T = int; int arg; // removes & 
        deduce1(cref_x); // T = int; int arg; // removes const & 
        deduce1(tab);    // T = int*; int* arg; // decays to pointer
        deduce1(foo);    // T = void(*)(int); void(*arg)(int)    // decays to pointer

        auto a1 = x;      // int
        auto a2 = cx;     // int
        auto a3 = ref_x;  // int
        auto a4 = cref_x; // int 
        auto a5 = tab;    // int*
        auto a6 = foo;    // void(*)(int)
        auto il = {1, 2, 3}; // std::initializer_list<int> - extra case
    }

    SECTION("Case 2")
    {
        deduce2(x);      // deduce2<T = int>(int& arg)   
        deduce2(cx);     // deduce2<T = const int>(const int& arg)   
        deduce2(ref_x);  // deduce2<T = int>(int&)
        deduce2(cref_x); // deduce2<T = const int>(const int&)
        deduce2(tab);    // deduce2<T = int[10]>(int(&arg)[10])
        deduce2(foo);    // deduce2<T = void(int)>(void(&arg)(int))

        auto& a1 = x;      // int&
        auto& a2 = cx;     // const int&
        auto& a3 = ref_x;  // int&
        auto& a4 = cref_x; // const int&
        auto& a5 = tab;    // int(&a5)[10]
        auto& a6 = foo;    // void(&a6)(int)
    }

    SECTION("Case 3")
    {
        deduce3(x);              
        deduce3(cx);             
        deduce3(ref_x);          
        deduce3(cref_x);         
        deduce3(tab);            
        deduce3(foo);            
        deduce3(string("text")); 

        auto&& a1 = x;
        auto&& a2 = cx;
        auto&& a3 = ref_x;
        auto&& a4 = cref_x;
        auto&& a5 = tab;
        auto&& a6 = foo;
        auto&& a7 = string("text");
    }
}

TEST_CASE("decltype")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];

    SECTION("deducing type")
    {
        decltype(x) a1 = 10;
        decltype(cx) a2 = 10;
        decltype(ref_x) a3 = x;
        decltype(cref_x) a4 = x;
        decltype(tab) a5 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    }

    //std::string s = "text";

    decltype(std::declval<std::string>().size()) str_size{};

    std::map<int, std::string> dict = { {1, "one"}, {2, "two"}};
    REQUIRE(dict.size() == 2);

    dict[3]; // evaluated
    REQUIRE(dict.size() == 3);

    decltype(dict[4]) item = dict[2]; // non-evaluated context
    REQUIRE(dict.size() == 3);
}

auto foobar() -> int
{
    return 42;
}

// int foobar()
// {

// }

template <typename T>
auto multiply(const T& a, const T& b) // -> decltype(a * b)
{
    return a * b;
}

template <typename F, typename Arg>
decltype(auto) call(F f, Arg arg) 
{
    std::cout << "Calling a function\n";
    return f(arg);
}

int square(int a)
{
    return a * a;
}

auto describe(int n)
{
    if (n % 2 == 0)
        return "even"s;
    return "odd"s;
}

template <typename T>
decltype(auto) get_nth(std::vector<T>& vec, size_t n) 
{
    assert(vec.size() > n);
    return vec[n];
}

TEST_CASE("calling a function")
{
    auto caller = call(square, 42);

    auto local_f = [](int a) -> double { return a * a; };

    std::vector<int> vec = {1, 2, 3, 4, 5};
    get_nth(vec, 1) = 42;
    REQUIRE(vec == std::vector{1, 42, 3, 4, 5});

    std::vector<std::string> words = {"one", "two" };
    get_nth(words, 1) = "dwa";
    REQUIRE(words == std::vector{"one"s, "dwa"s});

    std::vector<bool> flags = {0, 1, 1, 0};
    get_nth(flags, 1) = false;
    REQUIRE(flags == std::vector<bool>{0, 0, 1, 0});
}
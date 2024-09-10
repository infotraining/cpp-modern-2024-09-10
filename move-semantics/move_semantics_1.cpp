#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace std::literals;

std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

void foo(const std::string& text)
{
    std::cout << "lvalue: " << text << "\n";
}

void foo(std::string&& text)
{
    std::cout << "rvalue: " << text << "\n";
    foo(text);
}

TEST_CASE("lvalue vs. rvalue")
{
    std::string name = "jan"; // name is lvalue

    // foo(name);
    // foo(std::string("abc"));
    foo("text");
}

TEST_CASE("reference binding")
{
    std::string name = "jan"; // name is lvalue

    SECTION("C++98")
    {
        std::string& ref_name = name; // lvalue binds with lvalue-ref
        const std::string& ref_full_name = full_name(name, "Kowalski");
        //ref_full_name[0] = 'J';
    }

    SECTION("C++11")
    {
        std::string&& rvref_full_name = full_name(name, "Kowalski");
        rvref_full_name[0] = 'J';
        REQUIRE(rvref_full_name == "Jan Kowalski"s);

        //std::string&& rvref_name = name; // ERROR - cannot bind lvalue to rvalue-ref
    }
}

const int pi = 3.14;

auto txt = "text";
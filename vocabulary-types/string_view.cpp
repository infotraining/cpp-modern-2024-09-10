#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>

using namespace std::literals;

struct Length
{
    double value; 
};

Length operator""_m(long double value)
{
    return Length{static_cast<double>(value)};
}

Length operator""_cm(long double value)
{
    return Length{static_cast<double>(value) * 0.01};
}


double calculate(Length l)
{
    return l.value * 3.14;
}

TEST_CASE("custom literals")
{
    Length l1{10.11};
    auto result1 = calculate(l1);
    auto result2 = calculate(Length{10});

    auto result3 = calculate(10.55_m);
    auto result4 = calculate(10.55_cm);
}

void foobar(const std::string& text)
{
    std::cout << text << "\n";
}

void foobar(const char* text)
{
    std::cout << text << "\n";
}

namespace ModernCpp
{
    void foobar(std::string_view text)
    {
        std::cout << text << "\n";

        //fopen(text.c_str(), "rw+");  // BEWARE!!!
        std::string filename(text);
        fopen(filename.c_str(), "rw+");
    }
}


TEST_CASE("string_view")
{
    SECTION("default constructor")
    {
        std::string_view empty_sv;
        REQUIRE(empty_sv.data() == nullptr);
        REQUIRE(empty_sv.size() == 0);

        std::string empty_str;
        REQUIRE(empty_str.data() != nullptr);
    }

    SECTION("init with text")
    {
        const char* c_txt = "Hello World!";
        std::string str_text = "One Two";
        ModernCpp::foobar(c_txt);
        ModernCpp::foobar(str_text);

        std::string_view txt_sv = c_txt;
        REQUIRE(txt_sv == "Hello World"sv);

        std::string_view str_sv = str_text;
        REQUIRE(str_sv.starts_with("One"));

        std::string_view token{str_text.c_str(), 3};
        REQUIRE(token == "One"sv);

        std::array<char, 4> arr{'T', 'E', 'X', 'T'};
        std::string_view token_2{arr.data(), arr.size()};
        REQUIRE(token_2 == "TEXT");
    }    
}


std::string_view start_from_word(std::string_view text, std::string_view word)
{
      return text.substr(text.find(word));
}

std::string get_text()
{
    return "One Two Three";
}

TEST_CASE("string_view & lifetime issues")
{
    std::string str = "text";
    std::string_view str_sv = str;
    str += "!!!"; // creates dangling pointer in str_sv

    std::string_view dangling_sv = "text"s;

    auto result = start_from_word(get_text(), "One"); // result has dangling pointer!!
}

TEST_CASE("using string_view")
{
    std::vector<std::string> words = { "ala", "ma", "kota", "!" };

    std::vector<std::string_view> tokens(words.begin(), words.end());

    std::sort(tokens.begin(), tokens.end());
    std::ranges::sort(tokens, std::greater{}, [](const auto& t) { return t.size(); }); // with projection
    std::ranges::sort(tokens, [](const auto& t1, const auto& t2) { return t1.size() > t2.size(); });

    for(const auto& token : tokens)
    {
        std::cout << token << " ";
    }
}   
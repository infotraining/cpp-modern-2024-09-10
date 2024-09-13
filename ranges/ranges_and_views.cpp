#include <catch2/catch_test_macros.hpp>
#include <helpers.hpp>
#include <iostream>
#include <ranges>
#include <string>
#include <list>
#include <vector>
#include <deque>
#include <map>
#include <string_view>

using namespace std::literals;

template <auto Value>
struct Sentinel
{
    bool operator==(auto it) const
    {
        return *it == Value;
    }
};

TEST_CASE("ranges", "[ranges]")
{
    // auto data = helpers::create_numeric_dataset<20>(42);
    // helpers::print(data, "data");

    std::vector words = {"one"s, "two"s, "three"s, "four"s, "five"s, "six"s, "seven"s, "eight"s, "nine"s, "ten"s, 
                         "eleven"s, "twelve"s, "thirteen"s, "fourteen"s, "fifteen"s, "sixteen"s, "seventeen"s, "eighteen"s, "nineteen"s, "twenty"s};
    //helpers::print(words, "words");

    SECTION("algorithms")
    {
        std::ranges::sort(words, std::greater{});
        helpers::print(words, "words sorted");

        auto pos = std::ranges::find(words, 4, [](auto s) { return s.size(); });
        std::cout << "Found: " << *pos << "\n";
    }

    SECTION("projections")
    {
        // TODO        
    }

    SECTION("concepts & tools")
    {
        // TODO
    }
}

TEST_CASE("sentinels", "[ranges]")
{
    std::vector data = {2, 3, 4, 1, 5, 42, 6, 7, 8, 9, 10};

    // TODO - sort range [begin; 42) in descending order
    std::ranges::sort(data.begin(), Sentinel<42>{});

    auto pos = std::ranges::find(data.begin(), std::unreachable_sentinel, 42);

    helpers::print(data, "data");

    std::vector<int> target;

    std::ranges::copy(std::counted_iterator{data.begin(), 5}, 
        std::default_sentinel, 
        std::back_inserter(target));

    helpers::print(target, "target");

    //std::string str = "Hello!!!\0fhgaksdjhgkasdjh";

    //std::ranges::sort(str.begin(), Sentinel<'\0'>{}, std::greater{});

    // for(const auto& c : std::ranges::subrange(str.begin(), Sentinel<'\0'>{}))
    // {
    //     std::cout << c << " ";
    // }
    std::cout << "\n";
}

TEST_CASE("views")
{
    std::deque data = {2, 3, 4, 1, 5, 42, 6, 7, 8, 9, 10};

    SECTION("all")
    {
        auto all_items = std::views::all(data);
        helpers::print(all_items, "all_items");
    }

    SECTION("subrange - iterator & sentinel as a view")
    {
        auto head = std::ranges::subrange(data.begin(), data.begin() + 3);
        helpers::print(head, "head");
    }

    SECTION("counted")
    {        
        auto head_3 = std::views::counted(data.begin(), 3);
        helpers::print(head_3, "head_3");
    }

    SECTION("iota")
    {
        auto gen = std::views::iota(1);

        auto it = gen.begin();

        std::cout << *it << "\n";
        ++it;

        std::cout << *it << "\n";
        ++it;

        std::cout << *it << "\n";
        ++it;

        helpers::print(std::views::iota(1, 10), "iota");
    }

    SECTION("pipes |")
    {
        auto items = std::views::iota(1)
            | std::views::take(10)
            | std::views::transform([](int x) { return x * x;})
            | std::views::filter([](int x) { return x % 2 == 0; })
            | std::views::reverse;

        for(const auto item : items)
        {
            std::cout << item << "\n";
        }

        helpers::print(items, "items");

        std::map<int, std::string> dict = { {1, "one"}, {2, "two"}, {3, "three"} };

        helpers::print(dict | std::views::keys, "keys");
        helpers::print(dict | std::views::values | std::views::reverse, "values");
    }
}

TEST_CASE("views - reference semantics")
{    
    std::deque data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector words = { "one", "two", "three" };

    for(const auto& [number, word] : std::views::zip(data, words))
    {
        std::cout << number << " - " << word << "\n";
    }

    auto evens_view = data | std::views::filter([](int i) { return i % 2 == 0; });
    helpers::print(data, "data");

    for(auto& item : evens_view)
        item = 0;

    helpers::print(data, "data");
}

std::vector<std::string_view> tokenize(std::string_view text, char separator = ' ')
{
    auto tokens = text 
        | std::views::split(separator)
        | std::views::transform([](auto token) { return std::string_view(token.begin(), token.end()); });

    std::vector<std::string_view> vec_tokens(tokens.begin(), tokens.end());

    return vec_tokens;
}

TEST_CASE("tokenize")
{
    std::vector<std::string_view> tokens = tokenize("abc def ghi");

    helpers::print(tokens, "tokens");
}
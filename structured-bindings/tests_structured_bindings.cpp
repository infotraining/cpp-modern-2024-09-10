#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace std::literals;

TEST_CASE("tuple")
{
    std::tuple<int, std::string, double> tpl1{42, "text", 3.14};

    REQUIRE(std::get<0>(tpl1) == 42);
    REQUIRE(std::get<1>(tpl1) == "text"s);
    REQUIRE(std::get<2>(tpl1) == 3.14);

    const int x = 42;
    auto tpl2 = std::make_tuple(x, "text"s, 3.14);
    auto tpl3 = std::tuple{42, "text-text"s, 6.28};

    REQUIRE(tpl2 == tpl1);
    REQUIRE(tpl2 < tpl3);

    std::vector<decltype(tpl1)> vec{tpl1, tpl3, tpl2};
    std::ranges::sort(vec, [](const auto& t1, const auto& t2) {
        return std::get<1>(t1) < std::get<1>(t2);
    });

    int x1, y1;

    // std::tuple<int&, int&> ref_tpl(x1, y1);
    auto ref_tpl = std::tie(x1, y1);
    std::get<0>(ref_tpl) = 665;
    REQUIRE(x1 == 665);

    ref_tpl = std::tuple{22, 33};
    REQUIRE(x1 == 22);
    REQUIRE(y1 == 33);
}

tuple<int, int, double> calc_stats(const vector<int>& data)
{
    auto [pos_min, pos_max] = minmax_element(data.begin(), data.end());

    double avg = accumulate(data.begin(), data.end(), 0.0) / data.size();

    return make_tuple(*pos_min, *pos_max, avg);
}

TEST_CASE("Before C++17")
{
    vector<int> data = {4, 42, 665, 1, 123, 13};

    int min, max;
    //double avg;

    tie(min, max, std::ignore) = calc_stats(data);

    REQUIRE(min == 1);
    REQUIRE(max == Catch::Approx(665));
    //REQUIRE(avg == Catch::Approx(141.333));
}

struct Data
{
    int a;
    double b;
    std::vector<int> c;
};

Data create_data()
{
    return Data{1, 3.14, {1, 2, 3}};
}

TEST_CASE("Since C++17")
{
    vector<int> data = {4, 42, 665, 1, 123, 13};

    const auto [min, max, avg] = calc_stats(data);

    REQUIRE(min == 1);
    REQUIRE(max == Catch::Approx(665));
    REQUIRE(avg == Catch::Approx(141.333));

    SECTION("native arrays")
    {
        int coord[2] = {1, 2};
        auto& [x, y] = coord;

        REQUIRE(x == 1);
        REQUIRE(y == 2);
    }

    SECTION("struct")
    {
        auto[id, double_value, dataset] = create_data();
    }

    SECTION("how it works")
    {
        Data data{1, 3.14, {1, 2, 3}};

        const auto& [id, pi, dataset] = data;

        SECTION("is interpreted as")
        {
            const auto& entity_ = data;
            auto&& id = entity_.a;
            auto&& pi = entity_.b;
            auto&& dataset = entity_.c;
        }
    }
}

TEST_CASE("iteration over maps")
{
    std::map<int, std::string> dict = {{1, "one"}, {2, "two"}};

    for (const auto& [key, value] : dict)
    {
        std::cout << key << "-" << value << "\n";
    }

    auto [pos, was_inserted] = dict.emplace(4, "four");
    if (was_inserted)
    {
        const auto&[key, value] = *pos;
        std::cout << "Item (" << key << "-" << value << ") was inserted...\n";
    }
}

TEST_CASE("enumerate")
{
    std::vector vec = { "one", "two", "three" };

    
    for(auto [it, index] = std::tuple{vec.begin(), 0}; it != vec.end(); ++it, ++index)
    {
        std::cout << index << " " << *it << "\n";
    }
}
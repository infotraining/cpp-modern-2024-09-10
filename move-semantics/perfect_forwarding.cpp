#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <ranges>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

using namespace Helpers;

////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    g.use();
}

void have_fun(const Gadget& cg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    cg.use();
}

void have_fun(Gadget&& g)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    g.use();
}

namespace WithoutPerfectForwarding
{
    void use(Gadget& g)
    {
        have_fun(g);
    }

    void use(const Gadget& g)
    {
        have_fun(g);
    }

    void use(Gadget&& g)
    {
        have_fun(std::move(g));
    }
} // namespace WithoutPerfectForwarding

namespace PerfectForwarding
{
    template <typename TGadget = Gadget>
    void use(TGadget&& g)  // universal reference
    {
        have_fun(std::forward<TGadget>(g));
    }
}

namespace Explain
{
    template <typename T> 
    decltype(auto) forward(T&& obj)
    {
        if constexpr(std::is_lvalue_reference_v<T>)
        {
            return obj;
        }
        else
        {
            //return static_cast<T&&>(obj);
            return std::move(obj);
        }
    }
}

TEST_CASE("using gadget")
{
    Gadget g{1, "g"};
    const Gadget cg{2, "const g"};    

    using PerfectForwarding::use;

    use(g);  // g l-value
    use(cg); // cg l-value
    use(Gadget{3, "temporary gadget"}); // rvalue

    auto&& universal_ref1 = g;
    auto&& universal_ref2 = Gadget{3, "smartwatch"};
}

TEST_CASE("push_back vs. emplace_back")
{
    std::vector<Gadget> gadgets;

    gadgets.push_back(Gadget{1, "ipad"});
    gadgets.emplace_back(Gadget{1, "ipad"});
    gadgets.emplace_back(1, "smartwatch");
}

template <typename T>
void print(T&& items)  // universal reference
{
    std::cout << "[ ";
    for(const auto& item : items)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

TEST_CASE("C++20 + views + universal reference")
{
    constexpr int tab[5] = {1, 2, 3, 4, 5};   

    constexpr int sum_of_evens = [&tab] {
        auto evens = tab | std::views::filter([](int n) { return n % 2 == 0; });

        int sum{};
        for(const auto& e : evens)
            sum += e;
        return sum;
    }();

    std::vector vec = {6, 7, 42, 665, 2342, 23, 5345, 13, 42};


    auto pusher = [&vec](auto&& item) {
        vec.push_back(std::forward<decltype(item)>(item));
    };

    pusher(1);
    pusher(2);
    pusher(3);

    std::vector<Gadget> vec_gadgets;

    auto emplacer = [&vec_gadgets](auto&&... args) {
        vec_gadgets.emplace_back(std::forward<decltype(args)>(args)...);
    };

    emplacer(42, "g42");
    emplacer(665, "g665");

    print(tab);
    print(vec);

    auto evens = vec | std::views::filter([](int n) { return n % 2 == 0; });

    print(evens);
}
#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

struct BoundingBox
{
    int w, h;
};

struct Color
{
    uint8_t r, g, b;
};

struct Rect
{
    int w, h;
    Color color;

    void draw() const
    {
        std::cout << "Rect::draw()\n";
    }

    BoundingBox box() const noexcept
    {
        return BoundingBox{w, h};
    }
};

struct ColorRect : Rect
{
    Color color;

    Color get_color() const noexcept
    {
        return color;
    }

    void set_color(Color new_color)
    {
        color = new_color;
    }
};

// clang-format off
template <typename T>
concept Shape = requires(const T& obj)
{
    { obj.box() } noexcept -> std::same_as<BoundingBox>;
    obj.draw();
};
// clang-format on

template <typename T>
concept ShapeWithColor = Shape<T> && requires(T obj, Color color) {
    { obj.get_color() } -> std::same_as<Color>;
    obj.set_color(color);
};

static_assert(Shape<Rect>);
static_assert(Shape<ColorRect>);

template <Shape T>
void render(T& shp)
{
    std::cout << "render<Shape T>\n";
    shp.draw();
}

template <ShapeWithColor T>
void render(T& shp)
{
    std::cout << "render<ShapeWithColor T>\n";
    shp.set_color(Color(0, 0, 0));
    shp.draw();
}

TEST_CASE("concept subsumation")
{
    Rect r{10, 20, {255, 0, 0}};
    ColorRect cr{10, 20, {0, 255, 0}};

    render(r);
    render(cr);
}

void use(std::random_access_iterator auto it)
{
    std::cout << "use(std::random_access_iterator auto it)\n";
}

void use(std::contiguous_iterator auto it)
{
    std::cout << "use(std::contiguous_iterator auto it)\n";
}

TEST_CASE("iterators & subsumation")
{
    std::vector<int> vec;
    use(vec.begin());

    std::deque<int> dq;
    use(dq.begin());
}

class Person
{
    std::string name;
    int age;

public:
    Person(std::string n, int a)
        : name{n}
        , age{a}
    { }
};

struct Aggregate
{
    std::string name;
    int age;
};

TEST_CASE("uniform init syntax")
{
    Person p1("Jan", 23);
    Person p2{"Adam", 44};

    Aggregate agg1{"Ewa", 33};
    Aggregate agg2("Ola", 34);
}
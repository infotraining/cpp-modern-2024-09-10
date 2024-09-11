#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

using namespace helpers;

class Data
{
    std::string name_;
    int* data_;
    size_t size_;

public:
    using iterator = int*;
    using const_iterator = const int*;

    Data(std::string name, std::initializer_list<int> list)
        : name_{std::move(name)}
        , size_{list.size()}
    {
        data_ = new int[list.size()];
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }

    Data(const Data& other)
        : name_(other.name_)
        , size_(other.size_)
    {
        std::cout << "Data(" << name_ << ": cc)\n";
        data_ = new int[size_];
        std::copy(other.begin(), other.end(), data_);
    }

    Data& operator=(const Data& other)
    {
        Data temp(other);
        swap(temp);

        std::cout << "Data=(" << name_ << ": cc)\n";

        return *this;
    }

    Data(Data&& other) noexcept
        : name_{std::move(other.name_)}
        , data_{std::exchange(other.data_, nullptr)}
        , size_{std::exchange(other.size_, 0)}
    {
        std::cout << "Data(" << name_ << ": mv)\n";
    }

    Data& operator=(Data&& other)
    {
        Data temp = std::move(other);
        swap(temp);

        std::cout << "Data=(" << name_ << ": mv)\n";
        return *this;
    }

    ~Data() noexcept
    {
        delete[] data_;
    }

    void swap(Data& other)
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    iterator begin() noexcept
    {
        return data_;
    }

    iterator end() noexcept
    {
        return data_ + size_;
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }
};

namespace ModernCpp
{
    class Data
    {
        using TContainer = std::vector<int>;
        std::string name_;
        TContainer data_;

    public:
        using iterator = TContainer::iterator;
        using const_iterator = decltype(data_)::const_iterator;

        Data(std::string name, std::initializer_list<int> list)
            : name_{std::move(name)}
            , data_{list}
        {
            std::cout << "Data(" << name_ << ")\n";
        }

        void swap(Data& other)
        {
            name_.swap(other.name_);
            data_.swap(other.data_);
        }

        iterator begin() noexcept
        {
            return data_.begin();
        }

        iterator end() noexcept
        {
            return data_.end();
        }

        const_iterator begin() const noexcept
        {
            return data_.begin();
        }

        const_iterator end() const noexcept
        {
            return data_.end();
        }
    };

} // namespace ModernCpp

Data create_data_set()
{
    static int id_seed = 0;
    const int id = ++id_seed;

    Data ds{"data-set#" + std::to_string(id), {54, 6, 34, 235, 64356, 235, 23}};

    return ds;
}

void unsafe_foo()
{ }

int safe_foo() noexcept
{
    std::vector vec = {1, 2, 3};
    try
    {
        return vec.at(10);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }

    return 42;
}

TEST_CASE("noexcept")
{
    std::vector<Data> vec;

    for (int i = 0; i < 10; ++i)
    {
        vec.push_back(create_data_set());
        std::cout << "-------\n";
    }

    static_assert(not noexcept(unsafe_foo()));
    static_assert(noexcept(safe_foo()));
}

void weird() noexcept(noexcept(safe_foo()))
{
}

TEST_CASE("Data & move semantics")
{
    Data ds1 = create_data_set();

    Data backup = std::move(ds1); // move
    helpers::print(backup, "backup");
}

struct SuperDataSet
{
    std::string name;
    Data dataset;
    std::vector<double> numbers;

    SuperDataSet(std::string n, Data ds, std::vector<double> nmbs)
        : name{std::move(n)}
        , dataset{std::move(ds)}
        , numbers{std::move(nmbs)}
    { }

    // ~SuperDataSet() = default;
    // SuperDataSet(SuperDataSet&&) = default;
    // SuperDataSet& operator=(SuperDataSet&&) = default;
    // SuperDataSet(const SuperDataSet&) = default;
    // SuperDataSet& operator=(const SuperDataSet&) = default;
};

namespace Legacy
{
    SuperDataSet* create_sds()
    {
        SuperDataSet* ptr = new SuperDataSet({"SDS", Data{"data", {1, 2, 3, 4, 5}}, std::vector{1.0, 2.0, 3.1}});

        return ptr;
    }
} // namespace Legacy

SuperDataSet create_sds()
{
    std::vector<double> large_vec(1'000'000);
    return SuperDataSet{"sds", Data{"ds", {1, 2, 3}}, std::move(large_vec)};
}

TEST_CASE("SuperDataSet")
{
    SuperDataSet sds1{"SDS", Data{"data", {1, 2, 3, 4, 5}}, std::vector{1.0, 2.0, 3.1}};
    SuperDataSet sds2 = sds1;

    SuperDataSet target = std::move(sds1);

    SuperDataSet large_sds = create_sds();
}
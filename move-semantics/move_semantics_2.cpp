#include "gadget.hpp"
#include <utility>

#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////////////
// simplified implementation of unique_ptr - only moveable type

namespace Explain
{
    template <typename T>
    class unique_ptr
    {
    public:        
        unique_ptr(T* ptr = nullptr) : ptr_{ptr}
        {}

        ~unique_ptr()
        {
            delete ptr_;
        }

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        // move constructor
        // unique_ptr(unique_ptr&& source) : ptr_{source.ptr_}
        // {
        //     source.ptr_ = nullptr;
        // }

        unique_ptr(unique_ptr&& source) : ptr_{std::exchange(source.ptr_, nullptr)}
        {}

        // move assignment
        unique_ptr& operator=(unique_ptr&& source)
        {
            if (this != &source)
            {
                delete ptr_;
                // ptr_ = source.ptr_;
                // source.ptr_ = nullptr;
                ptr_ = std::exchange(source.ptr_, nullptr);
            }

            return *this;
        }

        explicit operator bool() const
        {
            return ptr_ != nullptr;
        }

        T& operator*() const
        {
            return *ptr_;
        }

        T* operator->() const
        {
            return ptr_;
        }

        T* get() const
        {
            return ptr_;
        }
    private:
        T* ptr_;
    };
} // namespace Explain

Explain::unique_ptr<Helpers::Gadget> create_gadget()
{
    static int id_seed = 0;
    const int id = ++id_seed;
    std::string name = "Gagdet#" + std::to_string(id);
    Explain::unique_ptr<Helpers::Gadget> ptr(new Helpers::Gadget(id, name));
    return ptr;
}

TEST_CASE("move semantics - unique_ptr")
{    
    using Helpers::Gadget;

    Explain::unique_ptr<Gadget> ptr_g = create_gadget();
    
    if (ptr_g)
    {
        ptr_g->use();
        (*ptr_g).use();
    }

    Explain::unique_ptr<Gadget> other_g = std::move(ptr_g);

    other_g = create_gadget();

    std::vector<Explain::unique_ptr<Gadget>> vec_g;

    vec_g.push_back(std::move(other_g));
    vec_g.push_back(create_gadget());
    vec_g.push_back(create_gadget());
    vec_g.push_back(create_gadget());
    vec_g.push_back(create_gadget());

    for(const auto& g : vec_g)
    {
        if (g)
            g->use();
    }
}
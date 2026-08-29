#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <memory>

namespace pdsa {

namespace detail 
{
    //
}

template <typename T> class vector 
{
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    using alloc_type = std::allocator<T>;
    using alloc_traits = std::allocator_traits<alloc_type>;
    alloc_type alloc;

public:
    vector(): data_(nullptr), size_(0), capacity_(0) {}
    vector(std::size_t count, const T &value): data_(nullptr), size_(0), capacity_(0)
    {
        if (count == 0)return;
        size_ = count;
        capacity_ = 1;
        while (capacity_ < count) capacity_ *= 2;
        data_ = alloc_traits::allocate(alloc, capacity_);
        for (std::size_t i = 0; i < count; i++) alloc_traits::construct(alloc, data_ + i, value);
    }
    ~vector() {
        if (size_ == 0) return;
        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        alloc_traits::deallocate(alloc, data_, capacity_);
    }

    std::size_t size() { return size_; }

    T &operator[](std::size_t i) { return data_[i]; }
};

} // namespace pdsa

#endif

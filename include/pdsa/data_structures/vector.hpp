#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iterator>
#include <cstddef>
#include <memory>

namespace pdsa {

namespace detail 
{
    //
}

template <typename T> 
class vector 
{
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    using alloc_type = std::allocator<T>;
    using alloc_traits = std::allocator_traits<alloc_type>;
    alloc_type alloc;

    void grow()
    {
        std::size_t new_capacity;
        if (capacity_ == 0) new_capacity = 1;
        else new_capacity = capacity_*2;
        T* new_ptr = alloc_traits::allocate(alloc, new_capacity);
        for(std::size_t i = 0; i < size_; i++) alloc_traits::construct(alloc, new_ptr + i, data_[i]);
        for(std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
        data_ = new_ptr;
        capacity_ = new_capacity;
    }

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
    ~vector()
    {
        if (capacity_ == 0) return;
        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        alloc_traits::deallocate(alloc, data_, capacity_);
    }

    struct Iterator;

    std::size_t size() const {return size_;}
    std::size_t capacity() const {return capacity_;}
    bool empty() const {return size_ == 0;}

    T &operator[](std::size_t i) {return data_[i];}

    Iterator begin() {return Iterator(data_);}
    Iterator end()
    {
        if (data_ == nullptr) return Iterator(nullptr);
        return Iterator(data_ + size_);
    }
};

template<typename T>
struct vector<T>::Iterator
{
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;

    Iterator(): m_ptr(nullptr) {}
    Iterator(pointer ptr): m_ptr(ptr) {}

    reference operator*() const {return *m_ptr;}
    pointer operator->() const {return m_ptr;}

    Iterator& operator++() {m_ptr++; return *this;}
    Iterator& operator--() {m_ptr--; return *this;}

    Iterator operator++(int) {Iterator tmp = *this; ++(*this); return tmp;}
    Iterator operator--(int) {Iterator tmp = *this; --(*this); return tmp;}

    friend bool operator==(const Iterator& a, const Iterator& b) {return a.m_ptr == b.m_ptr;}
    friend bool operator!=(const Iterator& a, const Iterator& b) {return a.m_ptr != b.m_ptr;}
    friend bool operator<(const Iterator& a, const Iterator& b) {return a.m_ptr < b.m_ptr;}
    friend bool operator<=(const Iterator& a, const Iterator& b) {return a.m_ptr <= b.m_ptr;}
    friend bool operator>(const Iterator& a, const Iterator& b) {return a.m_ptr > b.m_ptr;}
    friend bool operator>=(const Iterator& a, const Iterator& b) {return a.m_ptr >= b.m_ptr;}

    friend difference_type operator-(const Iterator& a, const Iterator& b) {return a.m_ptr - b.m_ptr;}

    Iterator& operator+=(difference_type n) {m_ptr += n; return *this;}
    Iterator& operator-=(difference_type n) {m_ptr -= n; return *this;}

    Iterator operator+(difference_type n) const {return Iterator(m_ptr + n);}
    Iterator operator-(difference_type n) const {return Iterator(m_ptr - n);}
    friend Iterator operator+(difference_type n, Iterator it) {return Iterator(it.m_ptr + n);}

    reference operator[](difference_type n) const {return *(m_ptr + n);}

private:
    pointer m_ptr;
};

} // namespace pdsa

#endif

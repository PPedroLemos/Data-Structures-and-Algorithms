#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace pdsa
{

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

    void grow()
    {
        std::size_t new_capacity;
        if (capacity_ == 0) new_capacity = 1;
        else new_capacity = capacity_ * 2;
        T* new_ptr = alloc_traits::allocate(alloc, new_capacity);
        for (std::size_t i = 0; i < size_; i++)
            alloc_traits::construct(alloc, new_ptr + i, data_[i]);
        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
        data_ = new_ptr;
        capacity_ = new_capacity;
    }

  public:
    vector() : data_(nullptr), size_(0), capacity_(0) {}
    vector(std::size_t count, const T& value) : data_(nullptr), size_(0), capacity_(0)
    {
        if (count == 0) return;
        capacity_ = 1;
        while (capacity_ < count) capacity_ *= 2;
        data_ = alloc_traits::allocate(alloc, capacity_);
        try
        {
            for (std::size_t i = 0; i < count; i++)
            {
                alloc_traits::construct(alloc, data_ + size_, value);
                ++size_;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
            alloc_traits::deallocate(alloc, data_, capacity_);
            throw;
        }
    }
    vector(std::size_t count) : data_(nullptr), size_(0), capacity_(0)
    {
        if (count == 0) return;
        capacity_ = 1;
        while (capacity_ < count) capacity_ *= 2;
        data_ = alloc_traits::allocate(alloc, capacity_);
        try
        {
            for (std::size_t i = 0; i < count; i++)
            {
                alloc_traits::construct(alloc, data_ + size_);
                ++size_;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
            alloc_traits::deallocate(alloc, data_, capacity_);
            throw;
        }
    }
    vector(std::initializer_list<T> initializer_list) : data_(nullptr), size_(0), capacity_(0)
    {
        if (initializer_list.size() == 0) return;
        capacity_ = 1;
        while (capacity_ < initializer_list.size()) capacity_ *= 2;
        data_ = alloc_traits::allocate(alloc, capacity_);
        try
        {
            for (const T& value : initializer_list)
            {
                alloc_traits::construct(alloc, data_ + size_, value);
                ++size_;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
            alloc_traits::deallocate(alloc, data_, capacity_);
            throw;
        }
    }
    ~vector()
    {
        if (capacity_ == 0) return;
        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        alloc_traits::deallocate(alloc, data_, capacity_);
    }

    bool operator==(const vector& other) const
    {
        if (this->size_ != other.size_) return false;
        for (std::size_t i = 0; i < this->size_; i++)
            if ((*this)[i] != other[i]) return false;
        return true;
    }

    // Copy

    vector(const vector& other) : size_(other.size_), capacity_(other.capacity_)
    {
        if (other.size_ != 0) data_ = alloc_traits::allocate(alloc, capacity_);
        else data_ = nullptr;
        for (std::size_t i = 0; i < size_; i++)
            alloc_traits::construct(alloc, data_ + i, other.data_[i]);
    }

    vector& operator=(const vector& other)
    {
        if (this == &other) return *this;

        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
        data_ = nullptr;
        if (other.data_ != nullptr) data_ = alloc_traits::allocate(alloc, other.capacity_);
        for (std::size_t i = 0; i < other.size_; i++)
            alloc_traits::construct(alloc, data_ + i, other.data_[i]);
        size_ = other.size_;
        capacity_ = other.capacity_;

        return *this;
    }

    // Move

    vector(vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    vector& operator=(vector&& other) noexcept
    {
        if (this == &other) return *this;

        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    struct Iterator;
    struct ConstIterator;

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    T* data() { return data_; }
    const T* data() const { return data_; }

    T& operator[](std::size_t i) { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }

    Iterator begin() { return Iterator(data_); }
    Iterator end()
    {
        if (data_ == nullptr) return Iterator(nullptr);
        return Iterator(data_ + size_);
    }

    ConstIterator begin() const { return ConstIterator(data_); }
    ConstIterator end() const
    {
        if (data_ == nullptr) return ConstIterator(nullptr);
        return ConstIterator(data_ + size_);
    }

    ConstIterator cbegin() const { return ConstIterator(data_); }
    ConstIterator cend() const
    {
        if (data_ == nullptr) return ConstIterator(nullptr);
        return ConstIterator(data_ + size_);
    }

    void push_back(const T& value)
    {
        if (size_ >= capacity_) grow();
        alloc_traits::construct(alloc, data_ + size_, value);
        size_++;
    }

    void pop_back()
    {
        if (size_ == 0) throw std::out_of_range("pdsa::vector::pop_back: vector is empty");
        alloc_traits::destroy(alloc, data_ + size_ - 1);
        size_--;
    }

    void clear()
    {

        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        size_ = 0;
    }

    void shrink_to_fit()
    {
        if (size_ == capacity_) return;
        T* new_ptr = nullptr;
        if (size_ != 0) new_ptr = alloc_traits::allocate(alloc, size_);
        for (std::size_t i = 0; i < size_; i++)
            alloc_traits::construct(alloc, new_ptr + i, data_[i]);
        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
        data_ = new_ptr;
        capacity_ = size_;
    }

    T& at(std::size_t i)
    {
        if (i >= size_) throw std::out_of_range("pdsa::vector::at: index out of range");
        return data_[i];
    }

    const T& at(std::size_t i) const
    {
        if (i >= size_) throw std::out_of_range("pdsa::vector::at: index out of range");
        return data_[i];
    }

    T& front()
    {
        if (empty()) throw std::out_of_range("pdsa::vector::front: vector is empty");
        return data_[0];
    }

    const T& front() const
    {
        if (empty()) throw std::out_of_range("pdsa::vector::front: vector is empty");
        return data_[0];
    }

    T& back()
    {
        if (empty()) throw std::out_of_range("pdsa::vector::back: vector is empty");
        return data_[size_ - 1];
    }

    const T& back() const
    {
        if (empty()) throw std::out_of_range("pdsa::vector::back: vector is empty");
        return data_[size_ - 1];
    }
};

template <typename T> struct vector<T>::Iterator
{
    friend struct ConstIterator;

    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;

    Iterator() : m_ptr(nullptr) {}
    Iterator(pointer ptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }

    Iterator& operator++()
    {
        m_ptr++;
        return *this;
    }
    Iterator& operator--()
    {
        m_ptr--;
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }
    Iterator operator--(int)
    {
        Iterator tmp = *this;
        --(*this);
        return tmp;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
    friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }
    friend bool operator<(const Iterator& a, const Iterator& b) { return a.m_ptr < b.m_ptr; }
    friend bool operator<=(const Iterator& a, const Iterator& b) { return a.m_ptr <= b.m_ptr; }
    friend bool operator>(const Iterator& a, const Iterator& b) { return a.m_ptr > b.m_ptr; }
    friend bool operator>=(const Iterator& a, const Iterator& b) { return a.m_ptr >= b.m_ptr; }

    friend difference_type operator-(const Iterator& a, const Iterator& b)
    {
        return a.m_ptr - b.m_ptr;
    }

    Iterator& operator+=(difference_type n)
    {
        m_ptr += n;
        return *this;
    }
    Iterator& operator-=(difference_type n)
    {
        m_ptr -= n;
        return *this;
    }

    Iterator operator+(difference_type n) const { return Iterator(m_ptr + n); }
    Iterator operator-(difference_type n) const { return Iterator(m_ptr - n); }
    friend Iterator operator+(difference_type n, Iterator it) { return Iterator(it.m_ptr + n); }

    reference operator[](difference_type n) const { return *(m_ptr + n); }

  private:
    pointer m_ptr;
};

template <typename T> struct vector<T>::ConstIterator
{
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const value_type*;
    using reference = const value_type&;

    ConstIterator() : m_ptr(nullptr) {}
    ConstIterator(pointer ptr) : m_ptr(ptr) {}

    ConstIterator(const vector::Iterator& it) : m_ptr(it.m_ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }

    ConstIterator& operator++()
    {
        m_ptr++;
        return *this;
    }
    ConstIterator& operator--()
    {
        m_ptr--;
        return *this;
    }

    ConstIterator operator++(int)
    {
        ConstIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    ConstIterator operator--(int)
    {
        ConstIterator tmp = *this;
        --(*this);
        return tmp;
    }

    friend bool operator==(const ConstIterator& a, const ConstIterator& b)
    {
        return a.m_ptr == b.m_ptr;
    }
    friend bool operator!=(const ConstIterator& a, const ConstIterator& b)
    {
        return a.m_ptr != b.m_ptr;
    }
    friend bool operator<(const ConstIterator& a, const ConstIterator& b)
    {
        return a.m_ptr < b.m_ptr;
    }
    friend bool operator<=(const ConstIterator& a, const ConstIterator& b)
    {
        return a.m_ptr <= b.m_ptr;
    }
    friend bool operator>(const ConstIterator& a, const ConstIterator& b)
    {
        return a.m_ptr > b.m_ptr;
    }
    friend bool operator>=(const ConstIterator& a, const ConstIterator& b)
    {
        return a.m_ptr >= b.m_ptr;
    }

    friend difference_type operator-(const ConstIterator& a, const ConstIterator& b)
    {
        return a.m_ptr - b.m_ptr;
    }

    ConstIterator& operator+=(difference_type n)
    {
        m_ptr += n;
        return *this;
    }
    ConstIterator& operator-=(difference_type n)
    {
        m_ptr -= n;
        return *this;
    }

    ConstIterator operator+(difference_type n) const { return ConstIterator(m_ptr + n); }
    ConstIterator operator-(difference_type n) const { return ConstIterator(m_ptr - n); }
    friend ConstIterator operator+(difference_type n, ConstIterator it)
    {
        return ConstIterator(it.m_ptr + n);
    }

    reference operator[](difference_type n) const { return *(m_ptr + n); }

  private:
    pointer m_ptr;
};

} // namespace pdsa

#endif

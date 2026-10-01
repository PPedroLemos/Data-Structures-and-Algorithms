#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <pdsa/utility.hpp>
#include <stdexcept>
#include <utility>

namespace pdsa
{

namespace detail
{
} // namespace detail

/**
@brief A vector with contiguous storage.

@tparam T Type of the elements stored by the vector.
*/

template <typename T> class vector
{
  private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    using alloc_type = std::allocator<T>;
    using alloc_traits = std::allocator_traits<alloc_type>;
    alloc_type alloc;

    void reallocate(std::size_t new_capacity)
    {
        T* new_ptr = nullptr;
        if (new_capacity != 0) new_ptr = alloc_traits::allocate(alloc, new_capacity);
        std::size_t copied_size = 0;
        try
        {
            while (copied_size < size_)
            {
                alloc_traits::construct(alloc, new_ptr + copied_size, data_[copied_size]);
                ++copied_size;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < copied_size; i++) alloc_traits::destroy(alloc, new_ptr + i);
            alloc_traits::deallocate(alloc, new_ptr, new_capacity);
            throw;
        }
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
            while (size_ < count)
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
            while (size_ < count)
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

    vector(const vector& other) : data_(nullptr), size_(0), capacity_(0)
    {
        if (other.data_ != nullptr) data_ = alloc_traits::allocate(alloc, other.capacity_);
        capacity_ = other.capacity_;
        try
        {
            while (size_ < other.size_)
            {
                alloc_traits::construct(alloc, data_ + size_, other.data_[size_]);
                ++size_;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
            if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
            throw;
        }
    }

    vector& operator=(const vector& other)
    {
        if (this == &other) return *this;

        T* new_data = nullptr;
        if (other.data_ != nullptr) new_data = alloc_traits::allocate(alloc, other.capacity_);
        std::size_t new_size = 0;
        try
        {
            while (new_size < other.size_)
            {
                alloc_traits::construct(alloc, new_data + new_size, other.data_[new_size]);
                ++new_size;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < new_size; i++) alloc_traits::destroy(alloc, new_data + i);
            if (new_data != nullptr) alloc_traits::deallocate(alloc, new_data, other.capacity_);
            throw;
        }

        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
        data_ = new_data;

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

    std::size_t max_size() const noexcept
    {
        const std::size_t max_alloc = alloc_traits::max_size(alloc);
        const std::size_t max_pointer =
            static_cast<std::size_t>(std::numeric_limits<std::ptrdiff_t>::max()) / sizeof(T);

        return pdsa::min<std::size_t>(max_alloc, max_pointer);
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
        if (size_ < capacity_)
        {
            alloc_traits::construct(alloc, data_ + size_, value);
            ++size_;
            return;
        }

        std::size_t new_capacity = pdsa::max<std::size_t>(1, 2 * capacity_);
        T* new_ptr = alloc_traits::allocate(alloc, new_capacity);
        std::size_t copied_size = 0;
        try
        {
            while (copied_size < size_)
            {
                alloc_traits::construct(alloc, new_ptr + copied_size, data_[copied_size]);
                ++copied_size;
            }
            alloc_traits::construct(alloc, new_ptr + copied_size, value);
            ++copied_size;
        }
        catch (...)
        {
            for (std::size_t i = 0; i < copied_size; i++) alloc_traits::destroy(alloc, new_ptr + i);
            alloc_traits::deallocate(alloc, new_ptr, new_capacity);
            throw;
        }

        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
        data_ = new_ptr;
        capacity_ = new_capacity;
        size_ = copied_size;
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
        reallocate(size_);
    }

    void reserve(std::size_t new_capacity)
    {
        if (new_capacity <= capacity_) return;
        reallocate(new_capacity);
    }

    void resize(std::size_t new_size)
    {
        if (new_size == size_) return;
        if (new_size < size_)
        {
            for (std::size_t i = new_size; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
            size_ = new_size;
            return;
        }

        std::size_t constructed_size = 0;

        if (new_size <= capacity_)
        {
            try
            {
                while (size_ + constructed_size < new_size)
                {
                    alloc_traits::construct(alloc, data_ + size_ + constructed_size);
                    constructed_size++;
                }
            }
            catch (...)
            {
                for (std::size_t i = size_; i < size_ + constructed_size; i++)
                    alloc_traits::destroy(alloc, data_ + i);
                throw;
            }
            size_ += constructed_size;
            return;
        }

        std::size_t new_capacity = pdsa::max<std::size_t>(1, capacity_);
        while (new_capacity < new_size) new_capacity *= 2;
        T* new_ptr = alloc_traits::allocate(alloc, new_capacity);
        try
        {
            while (constructed_size < size_)
            {
                alloc_traits::construct(alloc, new_ptr + constructed_size, data_[constructed_size]);
                ++constructed_size;
            }
            while (constructed_size < new_size)
            {
                alloc_traits::construct(alloc, new_ptr + constructed_size);
                ++constructed_size;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < constructed_size; i++)
                alloc_traits::destroy(alloc, new_ptr + i);
            alloc_traits::deallocate(alloc, new_ptr, new_capacity);
            throw;
        }

        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
        data_ = new_ptr;
        capacity_ = new_capacity;
        size_ = constructed_size;
    }

    void resize(std::size_t new_size, const T& value)
    {
        if (new_size == size_) return;
        if (new_size < size_)
        {
            for (std::size_t i = new_size; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
            size_ = new_size;
            return;
        }

        std::size_t constructed_size = 0;

        if (new_size <= capacity_)
        {
            try
            {
                while (size_ + constructed_size < new_size)
                {
                    alloc_traits::construct(alloc, data_ + size_ + constructed_size, value);
                    constructed_size++;
                }
            }
            catch (...)
            {
                for (std::size_t i = size_; i < size_ + constructed_size; i++)
                    alloc_traits::destroy(alloc, data_ + i);
                throw;
            }
            size_ += constructed_size;
            return;
        }

        T copied_value = value;
        std::size_t new_capacity = pdsa::max<std::size_t>(1, capacity_);
        while (new_capacity < new_size) new_capacity *= 2;
        T* new_ptr = alloc_traits::allocate(alloc, new_capacity);
        try
        {
            while (constructed_size < size_)
            {
                alloc_traits::construct(alloc, new_ptr + constructed_size, data_[constructed_size]);
                ++constructed_size;
            }
            while (constructed_size < new_size)
            {
                alloc_traits::construct(alloc, new_ptr + constructed_size, copied_value);
                ++constructed_size;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < constructed_size; i++)
                alloc_traits::destroy(alloc, new_ptr + i);
            alloc_traits::deallocate(alloc, new_ptr, new_capacity);
            throw;
        }

        for (std::size_t i = 0; i < size_; i++) alloc_traits::destroy(alloc, data_ + i);
        if (data_ != nullptr) alloc_traits::deallocate(alloc, data_, capacity_);
        data_ = new_ptr;
        capacity_ = new_capacity;
        size_ = constructed_size;
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

    Iterator erase(ConstIterator pos);
    Iterator erase(std::size_t pos);

    Iterator insert(ConstIterator pos, const T& value);
    Iterator insert(std::size_t pos, const T& value);
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

template <typename T> vector<T>::Iterator vector<T>::erase(vector<T>::ConstIterator pos)
{
    std::size_t position = pos - begin();
    for (vector<T>::Iterator i = Iterator(data_ + position) + 1; i < end(); i++)
        *(i - 1) = std::move(*i);

    alloc_traits::destroy(alloc, data_ + size_ - 1);
    --size_;

    return Iterator(data_ + position);
}

template <typename T> vector<T>::Iterator vector<T>::erase(std::size_t pos)
{
    if (pos >= size_) throw std::out_of_range("pdsa::vector::erase: index out of range");

    return erase(ConstIterator(data_ + pos));
}

template <typename T>
vector<T>::Iterator vector<T>::insert(vector<T>::ConstIterator pos, const T& value)
{
    std::size_t position = (empty() ? 0 : pos - begin());
    T copied_value = value;

    if (position == size_)
    {
        push_back(copied_value);
        return end() - 1;
    }

    if (size_ == capacity_)
    {
        std::size_t new_capacity = 2 * capacity_;
        T* new_ptr = alloc_traits::allocate(alloc, new_capacity);
        std::size_t new_size = 0;
        try
        {
            while (new_size < position)
            {
                alloc_traits::construct(alloc, new_ptr + new_size, at(new_size));
                new_size++;
            }
            alloc_traits::construct(alloc, new_ptr + new_size, value);
            ++new_size;
            while (new_size <= size_)
            {
                alloc_traits::construct(alloc, new_ptr + new_size, at(new_size - 1));
                new_size++;
            }
        }
        catch (...)
        {
            for (std::size_t i = 0; i < new_size; i++) alloc_traits::destroy(alloc, new_ptr + i);
            alloc_traits::deallocate(alloc, new_ptr, new_capacity);
            throw;
        }

        clear();
        alloc_traits::deallocate(alloc, data_, capacity_);

        data_ = new_ptr;
        size_ = new_size;
        capacity_ = new_capacity;
    }
    else
    {
        alloc_traits::construct(alloc, data_ + size_, copied_value);
        vector<T>::Iterator It_pos = end();
        while (It_pos > begin() + position)
        {
            try
            {
                *It_pos = std::move(*(It_pos - 1));
            }
            catch (...)
            {
                alloc_traits::destroy(alloc, data_ + size_);
                throw;
            }
            --It_pos;
        }
        try
        {
            *It_pos = copied_value;
        }
        catch (...)
        {
            alloc_traits::destroy(alloc, data_ + size_);
            throw;
        }
        ++size_;
    }

    return Iterator(data_ + position);
}

template <typename T> vector<T>::Iterator vector<T>::insert(std::size_t pos, const T& value)
{
    if (pos > size_) throw std::out_of_range("pdsa::vector::insert: index out of range");
    if (empty()) return insert(begin(), value);
    return insert(begin() + pos, value);
}

} // namespace pdsa

#endif

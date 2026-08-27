#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <memory>

namespace pdsa
{
    namespace detail
    {
        //
    }

    template<typename T>
    class vector
    {
    private:
        T* data_;
        std::size_t size_;
        std::size_t capacity_;
    public: 
        vector(): data_(nullptr), size_(0), capacity_(0) {}
        vector(std::size_t count,const T& value): data_(nullptr), size_(0), capacity_(0)
        {
            if (count == 0) return;
            size_ = count;
            capacity_= 1;
            while (capacity_ < count) capacity_ *= 2;
            data_ = new T[capacity_];
            for (std::size_t i = 0; i < count; i++) data_[i] = value;
        }
        ~vector()
        {
            delete[] data_;
        }
    };

}

#endif  
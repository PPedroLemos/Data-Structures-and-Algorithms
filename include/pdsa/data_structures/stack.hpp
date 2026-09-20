#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
namespace pdsa
{

template<typename T> class stack
{
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
public:
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const  { return size_ == 0; }

};

}

#endif

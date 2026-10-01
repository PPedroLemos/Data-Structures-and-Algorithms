#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <utility>
namespace pdsa
{

template<typename T>
void swap(T& a, T& b)
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

}






#endif

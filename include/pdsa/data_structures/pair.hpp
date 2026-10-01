#ifndef PAIR_HPP
#define PAIR_HPP

namespace pdsa
{

namespace detail
{

}

template<typename T1 ,typename T2>
struct pair
{
    T1 first;
    T2 second;

    pair(): first(T1()), second(T2()) {};
    pair(const T1& first, const T2& second): first(first), second(second) {}
    ~pair() {}

};

}


#endif

#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <iterator>
#include <pdsa/data_structures/pair.hpp>
#include <pdsa/utility.hpp>
#include <random>

namespace pdsa
{

namespace detail
{

template <std::random_access_iterator Iterator>
pdsa::pair<Iterator, Iterator> partition(Iterator begin, Iterator end, std::mt19937& generator)
{
    using Diff = typename std::iterator_traits<Iterator>::difference_type;
    using T = typename std::iterator_traits<Iterator>::value_type;
    std::uniform_int_distribution<Diff> range(0, end - begin - 1);
    T pivot_value = *(begin + range(generator));
    Iterator i = begin;
    Iterator lt = begin;
    Iterator gt = end;

    while (i != gt)
    {
        if (*i < pivot_value) pdsa::swap(*(i++), *(lt++));
        else if (*i > pivot_value) pdsa::swap(*i, *(--gt));
        else ++i;
    }

    return {lt, gt};
}

template <std::random_access_iterator Iterator>
void quicksort(Iterator begin, Iterator end, std::mt19937& generator)
{
    if (begin == end || end - begin <= 1) return;
    pair<Iterator, Iterator> equal_to_pivot = partition(begin, end, generator);
    quicksort(begin, equal_to_pivot.first, generator);
    quicksort(equal_to_pivot.second, end, generator);
}

} // namespace detail

template <std::random_access_iterator Iterator> void quicksort(Iterator begin, Iterator end)
{
    std::mt19937 generator(std::random_device{}());

    detail::quicksort(begin, end, generator);
}

} // namespace pdsa

#endif

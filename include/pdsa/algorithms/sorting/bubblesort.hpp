#ifndef PDSA_BUBBLE_SORT_HPP
#define PDSA_BUBBLE_SORT_HPP

#include <iterator>
#include <pdsa/utility.hpp>

namespace pdsa
{

template <std::random_access_iterator Iterator> void bubble_sort(Iterator begin, Iterator end)
{
    if (end - begin <= 1) return;
    for (Iterator upper_limit = end; upper_limit > begin; upper_limit--)
    {
        bool swap_done = false;
        for (Iterator i = begin + 1; i < upper_limit; i++)
            if (*i < *(i - 1))
            {
                pdsa::swap(*i, *(i - 1));
                swap_done = true;
            }
        if (!swap_done) return;
    }
}

} // namespace pdsa

#endif

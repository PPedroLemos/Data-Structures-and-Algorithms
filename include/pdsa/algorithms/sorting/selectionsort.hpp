#ifndef PDSA_SELECTION_SORT_HPP
#define PDSA_SELECTION_SORT_HPP

#include <iterator>
#include <pdsa/utility.hpp>

namespace pdsa
{

template <std::random_access_iterator Iterator> void selection_sort(Iterator begin, Iterator end)
{
    if (end - begin <= 1) return;
    for (Iterator i = begin; i < end - 1; i++)
    {
        Iterator min = i;
        for (Iterator j = i + 1; j < end; j++)
            if (*min > *j) min = j;
        if (i != min) pdsa::swap(*i, *min);
    }
}

} // namespace pdsa

#endif

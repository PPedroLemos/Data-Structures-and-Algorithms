#include <iterator>
#include <memory>

namespace pdsa
{

namespace detail
{

template<typename Src_Iterator, typename Dst_Iterator>
void merge(Src_Iterator src_begin, Src_Iterator src_middle, Src_Iterator src_end, Dst_Iterator dst_begin)
{
    Src_Iterator i = src_begin;
    Src_Iterator j = src_middle;
    Dst_Iterator k = dst_begin;
    Dst_Iterator dst_end = dst_begin + (src_end - src_begin);
    
    for (; k != dst_end; k++) 
    {
        if (i >= src_middle) *k = *j++;
        else if (j >= src_end) *k = *i++;
        else if (*j < *i) *k = *j++;
        else *k = *i++;
    }
}

template<typename Src_Iterator, typename Dst_Iterator>
void merge_sort(Src_Iterator src_begin, Src_Iterator src_end, Dst_Iterator dst_begin)
{
    if (src_end - src_begin <= 1) return;

    auto size = src_end - src_begin;

    Dst_Iterator dst_end = dst_begin + size;
    Dst_Iterator dst_middle = dst_begin + size/2;
    Src_Iterator src_middle = src_begin + size/2;

    merge_sort(dst_begin, dst_middle, src_begin);
    merge_sort(dst_middle, dst_end, src_middle);

    merge(dst_begin, dst_middle, dst_end, src_begin);
}

}

template<typename Iterator>
void merge_sort(Iterator begin, Iterator end)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    using alloc_type = std::allocator<T>;
    using alloc_traits = std::allocator_traits<alloc_type>;
    alloc_type alloc;

    auto size = end - begin;
    if (size <= 1) return;

    T* aux = alloc_traits::allocate(alloc, size);
    Iterator src = begin;
    T* dst = aux;
    while (src != end) alloc_traits::construct(alloc, dst++, *(src++));
    detail::merge_sort(begin, end, aux);
    for (T* it = aux; it != aux + size; it++) alloc_traits::destroy(alloc, it);
    alloc_traits::deallocate(alloc, aux, size);
}

}

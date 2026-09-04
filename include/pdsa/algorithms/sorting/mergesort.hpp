#include <iterator>
#include <memory>

namespace pdsa
{

namespace detail
{
template<typename Iterator>
void merge()
{

}

template<typename Iterator, typename Dst_Iterator>
void merge_sort(Iterator src_begin, Iterator src_end, Dst_Iterator dst_begin)
{
    
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

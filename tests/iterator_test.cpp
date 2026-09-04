#include <pdsa/data_structures/vector.hpp>
#include <concepts>
#include <iterator>
#include <algorithm>

using It = pdsa::vector<int>::Iterator;
static_assert(std::random_access_iterator<It>);

int main()
{
    
}

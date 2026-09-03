#include <pdsa/data_structures/linear/vector.hpp>
#include <concepts>
#include <iterator>
#include <algorithm>
#include <random>

using It = pdsa::vector<int>::Iterator;
static_assert(std::random_access_iterator<It>);

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(1, 100);

int main()
{
    
}

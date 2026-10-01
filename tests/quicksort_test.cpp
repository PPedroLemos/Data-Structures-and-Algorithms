#include <iostream>
#include <random>
#include <pdsa/algorithms/sorting/quicksort.hpp>
#include <pdsa/data_structures/vector.hpp>

const std::size_t SIZE = 10000;
const int MAX = 1000;

std::mt19937 generator(std::random_device{}());
std::uniform_int_distribution<int> range(0, MAX);

int main()
{
    pdsa::vector<int> values;
    values.reserve(SIZE);

    for (std::size_t i = 0; i < SIZE; i++) values.push_back(range(generator));

    pdsa::quicksort(values.begin(), values.end());

    for (std::size_t i = 1; i< SIZE; i++)
    {
        if (values[i] < values[i-1]) 
        {
            std::cout << "NOT_SORTED\n";
            return 1;
        }
    }

    std::cout << "SORTED\n";

    for (std::size_t i = 0; i < SIZE; i ++) std::cout << values[i] << " ";
    std::cout << std::endl;

    return 0;
}

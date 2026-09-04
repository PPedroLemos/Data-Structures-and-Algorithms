#include <random>
#include <pdsa/algorithms/sorting/mergesort.hpp>
#include <pdsa/data_structures/vector.hpp>
#include <iostream>

const int SIZE = 100;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(1, 100);

void print_vector( pdsa::vector<int>& vec)
{
    for (int& x : vec) 
    {
         std::cout << x << " ";
    }
    std::cout << "\n";
}

int main()
{
    pdsa::vector<int> numbers(SIZE, 0);
    for (int& x : numbers) 
    {
        x = dist(gen);
    }

    std::cout << "Initial:\n";
    print_vector(numbers);

    pdsa::merge_sort(numbers.begin(), numbers.end());

    std::cout << "After sorting:\n";
    print_vector(numbers);

    for (int i = 1; i < numbers.size(); i++) 
    {
        if (numbers[i-1] > numbers[i])
        {
            std::cout << "NOT SORTED\n\n";
            return 1;
        }
    }
    
    std::cout << "SORTED\n\n";

    return 0;
}

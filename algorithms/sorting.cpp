#include "benchmark.hpp"

// clang-format off
SORTING_ALGORITHM(SelectionSort, {
    {
        int &n = this->n;
        auto &elements = this->elements;
        for (int i = 0; i < (n - 1); i++)
        {
            int minimum = i;
            // Scan through the array and find the minimum element
            for (int j = i + 1; j < n; j++)
            {
                if (elements[j] < elements[minimum])
                    minimum = j;
            }
            std::swap(elements[i], elements[minimum]);
        }
    }
})

SORTING_ALGORITHM(BubbleSort, {
    {
        int &n = this->n;
        auto &elements = this->elements;
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (elements[j] > elements[j + 1])
                    std::swap(elements[j], elements[j + 1]);
            }
        }
    }
})

SORTING_ALGORITHM(InsertionSort, {})
// clang-format on
int main()
{
    print_headers();
    std::vector<std::shared_ptr<Benchmarkable<int>>> sorting_algorithms = {
        std::make_shared<SelectionSort<int>>(), std::make_shared<BubbleSort<int>>()};

    for (auto algorithm : sorting_algorithms)
    {
        int number_of_elements = INITIAL_NUMBER_OF_ELEMENTS;
        for (int i = 0; i < NUMBER_OF_ITERATIONS; i++)
        {
            benchmark<int>(algorithm, number_of_elements);
            number_of_elements += INCREMENT_PER_ITERATION;
        }
    }
}

/*
| Sorting algorithm              | Input size | Status  | Time elapsed |
|--------------------------------|------------|---------|--------------|
| Selection sort                 |      20000 | SUCCESS |     1.2266 s |
| Selection sort                 |      25000 | SUCCESS |     1.7856 s |
| Selection sort                 |      30000 | SUCCESS |     2.5599 s |
| Selection sort                 |      35000 | SUCCESS |     3.7642 s |
| Selection sort                 |      40000 | SUCCESS |     4.7005 s |
| Selection sort                 |      45000 | SUCCESS |     6.0159 s |
| Selection sort                 |      50000 | SUCCESS |     7.5327 s |
| Selection sort                 |      55000 | SUCCESS |     9.4318 s |
| Bubble sort                    |      20000 | SUCCESS |     3.4518 s |
| Bubble sort                    |      25000 | SUCCESS |     5.6877 s |
| Bubble sort                    |      30000 | SUCCESS |     8.2749 s |
| Bubble sort                    |      35000 | SUCCESS |    11.3999 s |
| Bubble sort                    |      40000 | SUCCESS |    14.4376 s |
| Bubble sort                    |      45000 | SUCCESS |    16.9142 s |
| Bubble sort                    |      50000 | SUCCESS |    20.7272 s |
| Bubble sort                    |      55000 | SUCCESS |    24.8547 s |
*/
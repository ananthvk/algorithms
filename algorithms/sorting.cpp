#include "benchmark.hpp"

// clang-format off
SORTING_ALGORITHM(SelectionSort, {
    {
        int &n = this->n;
        auto &elements = this->elements;
        for (int i = 0; i < (n - 1); i++)
        {
            // Invariant: Elements to the left of i are in sorted order
            //            In the ith iteration, elements[i] is the minimum element of the right of the array
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

SORTING_ALGORITHM(InsertionSort, {
    int &n = this->n;
    auto &elements = this->elements;
    for(int i = 0; i < n; i++)
    {
        // Elements to the left of i, including i are in non decreasing order
        // If they are not, repeatedly swap elements[i] with elements[i-1]
        for(int j = i; j > 0; j--)
        {
            if(elements[j] < elements[j-1])
                std::swap(elements[j], elements[j-1]);
            else
                break;
        }
    }
})

// clang-format on

template <typename T> class NaiveMergeSort : public SortingAlgorithm<T>
{
  public:
    std::string name() const { return "NaiveMergeSort"; }

    void merge(std::vector<T> &b, std::vector<T> &c, std::vector<T> &a)
    {
        int i = 0, j = 0, k = 0;
        while (i < b.size() && j < c.size())
        {
            if (b[i] <= c[j])
                a[k++] = b[i++];
            else
                a[k++] = c[j++];
        }
        while (i < b.size())
            a[k++] = b[i++];
        while (j < c.size())
            a[k++] = c[j++];
    }

    void mergesort(std::vector<T> &a)
    {
        if (a.size() > 1)
        {
            std::vector<T> b;
            std::vector<T> c;
            std::copy(a.begin(), a.begin() + std::ceil(a.size() / 2.0), std::back_inserter(b));
            std::copy(a.begin() + std::ceil(a.size() / 2.0), a.end(), std::back_inserter(c));
            mergesort(b);
            mergesort(c);
            merge(b, c, a);
        }
    }

    void run()
    {
        int &n = this->n;
        auto &elements = this->elements;
        mergesort(elements);
    }
};

template <typename T> class QuickSort : public SortingAlgorithm<T>
{
  public:
    std::string name() const { return "QuickSort"; }

    int partititon(std::vector<T> &A, int p, int r)
    {
        auto x = A[r];
        auto i = p - 1;
        for (int j = p; j <= r - 1; j++)
        {
            if (A[j] <= x)
            {
                i = i + 1;
                std::swap(A[i], A[j]);
            }
        }
        std::swap(A[i + 1], A[r]);
        return i + 1;
    }

    void quicksort(std::vector<T> &a, int p, int r)
    {
        if (p < r)
        {
            auto q = partititon(a, p, r);
            quicksort(a, p, q - 1);
            quicksort(a, q + 1, r);
        }
    }

    void run()
    {
        int &n = this->n;
        auto &elements = this->elements;
        quicksort(elements, 0, elements.size() - 1);
    }
};

template <typename T> class ImprovedQuickSort : public SortingAlgorithm<T>
{
  public:
    std::string name() const { return "ImprovedQuickSort"; }

    int partititon(std::vector<T> &A, int p, int r)
    {
        auto x = A[r];
        auto i = p - 1;
        for (int j = p; j <= r - 1; j++)
        {
            if (A[j] <= x)
            {
                i = i + 1;
                std::swap(A[i], A[j]);
            }
        }
        std::swap(A[i + 1], A[r]);
        return i + 1;
    }

    void quicksort(std::vector<T> &a, int p, int r)
    {
        // If less than 7 elements are being sorted, apply insertion sort
        if ((p < r) && (r - p) < 7)
        {
            for (int i = p; i <= r; i++)
            {
                // Elements to the left of i, including i are in non decreasing order
                // If they are not, repeatedly swap elements[i] with elements[i-1]
                for (int j = i; j > p; j--)
                {
                    if (a[j] < a[j - 1])
                        std::swap(a[j], a[j - 1]);
                    else
                        break;
                }
            }
            return;
        }
        if (p < r)
        {
            auto q = partititon(a, p, r);
            quicksort(a, p, q - 1);
            quicksort(a, q + 1, r);
        }
    }

    void run()
    {
        int &n = this->n;
        auto &elements = this->elements;
        quicksort(elements, 0, elements.size() - 1);
    }
};


SORTING_ALGORITHM(STLSort, {
    auto &elements = this->elements;
    std::sort(elements.begin(), elements.end());
})

#define I(algorithm) std::make_shared<algorithm<int>>()

int main()
{
    print_headers();
    // std::vector<std::shared_ptr<Benchmarkable<int>>> sorting_algorithms = { I(MergeSort),
    // I(SelectionSort), I(BubbleSort), I(InsertionSort)};
    std::vector<std::shared_ptr<Benchmarkable<int>>> sorting_algorithms = {
        I(ImprovedQuickSort),
        I(QuickSort),
        I(STLSort),
    };

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

// $ g++ -O0 sorting.cpp && time ./a.out
// | Sorting algorithm              | Input size | Status  | Time elapsed |
// |--------------------------------|------------|---------|--------------|
// | InsertionSort                  |       5000 | SUCCESS |     0.1052 s |
// | InsertionSort                  |      10000 | SUCCESS |     0.4426 s |
// | InsertionSort                  |      15000 | SUCCESS |     0.9753 s |
// | InsertionSort                  |      20000 | SUCCESS |     1.7403 s |
// | InsertionSort                  |      25000 | SUCCESS |     2.6763 s |
// | InsertionSort                  |      30000 | SUCCESS |     3.8476 s |
// | InsertionSort                  |      35000 | SUCCESS |     5.2477 s |
// | InsertionSort                  |      40000 | SUCCESS |     7.0233 s |
// | InsertionSort                  |      45000 | SUCCESS |     9.5983 s |
// | InsertionSort                  |      50000 | SUCCESS |    11.0764 s |
// | SelectionSort                  |       5000 | SUCCESS |     0.0653 s |
// | SelectionSort                  |      10000 | SUCCESS |     0.2584 s |
// | SelectionSort                  |      15000 | SUCCESS |     0.5881 s |
// | SelectionSort                  |      20000 | SUCCESS |     1.0307 s |
// | SelectionSort                  |      25000 | SUCCESS |     1.6293 s |
// | SelectionSort                  |      30000 | SUCCESS |     2.2666 s |
// | SelectionSort                  |      35000 | SUCCESS |     3.1457 s |
// | SelectionSort                  |      40000 | SUCCESS |     4.1558 s |
// | SelectionSort                  |      45000 | SUCCESS |     5.2025 s |
// | SelectionSort                  |      50000 | SUCCESS |     6.4147 s |
// | BubbleSort                     |       5000 | SUCCESS |     0.1879 s |
// | BubbleSort                     |      10000 | SUCCESS |     0.7646 s |
// | BubbleSort                     |      15000 | SUCCESS |     1.7235 s |
// | BubbleSort                     |      20000 | SUCCESS |     3.0916 s |
// | BubbleSort                     |      25000 | SUCCESS |     4.8103 s |
// | BubbleSort                     |      30000 | SUCCESS |     6.9140 s |
// | BubbleSort                     |      35000 | SUCCESS |     9.4613 s |
// | BubbleSort                     |      40000 | SUCCESS |    12.6632 s |
// | BubbleSort                     |      45000 | SUCCESS |    15.6660 s |
// | BubbleSort                     |      50000 | SUCCESS |    19.3984 s |
// ./a.out  142.27s user 0.01s system 99% cpu 2:22.74 total


// $ g++ -O0 sorting.cpp && time ./a.out
// | Sorting algorithm              | Input size | Status  | Time elapsed |
// |--------------------------------|------------|---------|--------------|
// | ImprovedQuickSort              |     100000 | SUCCESS |     0.0268 s |
// | ImprovedQuickSort              |    1100000 | SUCCESS |     0.3801 s |
// | ImprovedQuickSort              |    2100000 | SUCCESS |     0.7960 s |
// | ImprovedQuickSort              |    3100000 | SUCCESS |     1.1351 s |
// | ImprovedQuickSort              |    4100000 | SUCCESS |     1.5748 s |
// | ImprovedQuickSort              |    5100000 | SUCCESS |     2.0286 s |
// | ImprovedQuickSort              |    6100000 | SUCCESS |     2.4258 s |
// | ImprovedQuickSort              |    7100000 | SUCCESS |     3.0087 s |
// | ImprovedQuickSort              |    8100000 | SUCCESS |     3.7045 s |
// | ImprovedQuickSort              |    9100000 | SUCCESS |     4.0607 s |
// | QuickSort                      |     100000 | SUCCESS |     0.0322 s |
// | QuickSort                      |    1100000 | SUCCESS |     0.4424 s |
// | QuickSort                      |    2100000 | SUCCESS |     0.8527 s |
// | QuickSort                      |    3100000 | SUCCESS |     1.2604 s |
// | QuickSort                      |    4100000 | SUCCESS |     1.6645 s |
// | QuickSort                      |    5100000 | SUCCESS |     2.2156 s |
// | QuickSort                      |    6100000 | SUCCESS |     2.5522 s |
// | QuickSort                      |    7100000 | SUCCESS |     3.0453 s |
// | QuickSort                      |    8100000 | SUCCESS |     3.4703 s |
// | QuickSort                      |    9100000 | SUCCESS |     3.9664 s |
// | STLSort                        |     100000 | SUCCESS |     0.0356 s |
// | STLSort                        |    1100000 | SUCCESS |     0.4711 s |
// | STLSort                        |    2100000 | SUCCESS |     0.9395 s |
// | STLSort                        |    3100000 | SUCCESS |     1.4169 s |
// | STLSort                        |    4100000 | SUCCESS |     1.8884 s |
// | STLSort                        |    5100000 | SUCCESS |     2.4612 s |
// | STLSort                        |    6100000 | SUCCESS |     2.9431 s |
// | STLSort                        |    7100000 | SUCCESS |     3.3857 s |
// | STLSort                        |    8100000 | SUCCESS |     3.8877 s |
// | STLSort                        |    9100000 | SUCCESS |     4.5205 s |
// ./a.out  82.17s user 0.07s system 99% cpu 1:22.49 total
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <sstream>
#include <time.h>
#include <vector>

#define INITIAL_NUMBER_OF_ELEMENTS 20000
#define NUMBER_OF_ITERATIONS 8
#define INCREMENT_PER_ITERATION 5000
#define VERBOSE_ERRORS false

template <typename T> T get_random_value(T min, T max)
{
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_int_distribution<T> dist(min, max);
    return dist(mt);
}

template <typename Iter> void shuffle(Iter begin, Iter end)
{
    if (end <= begin)
        return;
    for (auto it = end - 1; it != begin; it--)
    {
        auto idx = get_random_value<size_t>(0, (it - begin));
        std::swap(*(begin + idx), *it);
    }
}

// Generate a sequence from 1 to n, and returns it after shuffling
template <typename T> std::vector<T> generate_sequence(size_t n)
{
    std::vector<T> v(n);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

template <typename T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
    return os << std::endl;
}

template <typename T> class Benchmarkable
{
  public:
    virtual std::string name() const = 0;
    virtual void init(int n) = 0;
    virtual void run() = 0;
    virtual void destroy() = 0;
    virtual std::string verify() const = 0;

    virtual ~Benchmarkable() {}
};

template <typename T> class SelectionSort : public Benchmarkable<T>
{
    std::vector<T> elements;
    int n;

  public:
    std::string name() const { return "Selection sort"; }

    void init(int num)
    {
        n = num;
        elements = generate_sequence<T>(n);
        shuffle(elements.begin(), elements.end());
    }

    void run()
    {
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

    std::string verify() const
    {
        if (std::is_sorted(elements.begin(), elements.end()))
            return "";

        if (VERBOSE_ERRORS)
        {
            // To find the index where the error is occuring
            for (int i = 0; i < n; i++)
            {
                if (elements[i] != (i + 1))
                {
                    std::stringstream ss;
                    ss << "Verification failed: Expected " << (i + 1) << " at index " << i
                       << ", found " << elements[i];
                    return ss.str();
                }
            }
        }
        return "-";
    }

    void destroy() {}
};

template <typename T> class BubbleSort : public Benchmarkable<T>
{
    std::vector<T> elements;
    int n;

  public:
    std::string name() const { return "Bubble sort"; }

    void init(int num)
    {
        n = num;
        elements = generate_sequence<T>(n);
        shuffle(elements.begin(), elements.end());
    }

    void run()
    {
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (elements[j] > elements[j + 1])
                    std::swap(elements[j], elements[j + 1]);
            }
        }
    }

    std::string verify() const
    {
        if (std::is_sorted(elements.begin(), elements.end()))
            return "";

        // To find the index where the error is occuring
        if (VERBOSE_ERRORS)
        {
            for (int i = 0; i < n; i++)
            {
                if (elements[i] != (i + 1))
                {
                    std::stringstream ss;
                    ss << "Verification failed: Expected " << (i + 1) << " at index " << i
                       << ", found " << elements[i];
                    return ss.str();
                }
            }
        }
        return "-";
    }

    void destroy() {}
};

template <typename T> void benchmark(Benchmarkable<T> &b, int n)
{
    std::cout << "| " << std::setw(30) << std::left << b.name() << " | " << std::setw(10)
              << std::right << n << " | ";
    b.init(n);
    clock_t start = clock();
    b.run();
    double elapsed = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
    auto message = b.verify();
    b.destroy();
    if (!message.empty())
    {
        if (VERBOSE_ERRORS)
            std::cerr << "FAILED  | Verification failed: " << message << " |" << std::endl;
        else
            std::cerr << "FAILED  |   " << std::setw(10) << message << " |" << std::endl;
    }
    else
    {
        std::cout << "SUCCESS | " << std::setw(10) << std::right << std::setprecision(4)
                  << std::fixed << elapsed << " s |" << std::endl;
    }
}

void print_headers()
{
    // Print headers for the table
    std::cout << "| " << std::setw(30) << std::left << "Sorting algorithm" << " | " << std::setw(10)
              << std::right << "Input size" << " | " << "Status  |" << std::setw(10)
              << " Time elapsed" << " |" << std::endl;

    std::cout << "|-" << std::setw(30) << std::setfill('-') << std::left << "-----------------"
              << "-|-" << std::setw(10) << std::right << "----------" << "-|-" << "--------|"
              << std::setw(10) << "--------------|" << std::endl;
    std::cout << std::setfill(' ');
}

int main()
{
    print_headers();
    std::vector<std::shared_ptr<Benchmarkable<int>>> sorting_algorithms;
    sorting_algorithms.push_back(std::make_shared<SelectionSort<int>>());
    sorting_algorithms.push_back(std::make_shared<BubbleSort<int>>());

    for (auto algorithm : sorting_algorithms)
    {
        int number_of_elements = INITIAL_NUMBER_OF_ELEMENTS;
        for (int i = 0; i < NUMBER_OF_ITERATIONS; i++)
        {
            benchmark<int>(*algorithm, number_of_elements);
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
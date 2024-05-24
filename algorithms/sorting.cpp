#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <time.h>
#include <vector>

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

        // To find the index where the error is occuring
        for (int i = 0; i < n; i++)
        {
            if (elements[i] != (i + 1))
            {
                std::stringstream ss;
                ss << "Verification failed: Expected " << (i + 1) << " at index " << i << ", found "
                   << elements[i];
                return ss.str();
            }
        }
        return "";
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
        for (int i = 0; i < n; i++)
        {
            if (elements[i] != (i + 1))
            {
                std::stringstream ss;
                ss << "Verification failed: Expected " << (i + 1) << " at index " << i << ", found "
                   << elements[i];
                return ss.str();
            }
        }
        return "";
    }

    void destroy() {}
};

template <typename T> void benchmark(Benchmarkable<T> &b, int n)
{
    std::cout << "| "<<std::setw(30) << std::left << b.name() << " | " << std::setw(10) << std::right << n
              << " | ";
    b.init(n);
    clock_t start = clock();
    b.run();
    double elapsed = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
    auto message = b.verify();
    b.destroy();
    if (!message.empty())
    {
        std::cerr << "FAILED  | Verification failed: " << message << " |"<< std::endl;
    }
    else
    {
        std::cout << "SUCCESS | " << std::setw(10) << std::right << std::setprecision(4)
                  << std::fixed << elapsed << " s |" << std::endl;
    }
}

int main()
{
    SelectionSort<int> s;
    BubbleSort<int> b;
    int n = 20000;

    std::cout << "| "<< std::setw(30) << std::left << "Sorting algorithm" << " | " << std::setw(10) << std::right << "Input size" 
              << " | " << "Status  |" << std::setw(10) << " Time elapsed" << " |" << std::endl;

    std::cout << "|-" << std::setw(30) << std::setfill('-') << std::left << "-----------------" << "-|-" << std::setw(10) << std::right << "----------" 
              << "-|-" << "--------|" << std::setw(10) << "--------------|" << std::endl;
    std::cout << std::setfill(' ');
    for (int i = 0; i < 8; i++)
    {
        benchmark<int>(s, n);
        benchmark<int>(b, n);
        n += 5000;
    }
}
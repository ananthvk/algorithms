#include <algorithm>
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
    virtual std::string verify() const = 0;

    virtual ~Benchmarkable() {}
};

template <typename T> class SelectionSort : public Benchmarkable<T>
{
    std::vector<T> elements;
    int n;

  public:
    std::string name() const { return "Selection sort"; }

    void init(int n)
    {
        this->n = n;
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
        for (int i = 0; i < n; i++)
        {
            if (elements[i] != (i + 1))
            {
                std::stringstream ss;
                ss << "Verification failed: Expected " << (i + 1) << " at index " << i << ", found "
                   << elements[i] << std::endl;
                return ss.str();
            }
        }
        return "";
    }
};

template <typename T> void benchmark(Benchmarkable<T> &b, int n)
{
    std::cout << b.name() << " (" << n << ") : ";
    b.init(n);
    clock_t start = clock();
    b.run();
    double elapsed = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
    auto message = b.verify();
    if (!message.empty())
    {
        std::cerr << "FAILED: Verification failed: " << message << std::endl;
    }
    else
    {
        std::cout << "SUCCESS: Time elapsed: " << elapsed << std::endl;
    }
}

int main()
{
    SelectionSort<int> s;
    benchmark<int>(s, 1000);
}
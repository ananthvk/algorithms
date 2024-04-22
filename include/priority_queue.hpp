#pragma once
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T, typename Container = std::vector<T>,
          typename Compare = std::less<typename Container::value_type>>
class PriorityQueue
{
  private:
    // This implementation uses a max heap
    // For a node i, it's left child is at index 2*i + 1 and right child at 2*i + 2

    Container container;

    size_t parent(size_t i)
    {
        if (i % 2 == 0)
            return i / 2 - 1;
        else
            return i / 2;
    }

  public:
    PriorityQueue() {}

    typedef T value_type;
    typedef T &reference;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef Container container_type;
    typedef Compare value_compare;

    const_reference top() const { return container.front(); }

    size_type size() const { return container.size(); }

    bool empty() const { return container.empty(); }

    void push(const value_type &value)
    {
        container.push_back(value);
        size_t i = container.size() - 1;
        while (i > 0 && Compare{}(container[parent(i)], container[i]))
        {
            std::swap(container[i], container[parent(i)]);
            i = parent(i);
        }
    }

    void pop()
    {
        std::swap(container[0], container.back());
        container.pop_back();
        size_t i = 0;
        while ((2 * i + 1) < size())
        {
            reference left = container[2 * i + 1];
            reference right = ((2 * i + 2) < size()) ? container[2 * i + 2] : left;
            reference ref = !Compare{}(left, right) ? left : right;
            if (!Compare{}(container[i], ref))
                break;

            std::swap(container[i], ref);

            if (std::addressof(ref) == std::addressof(left))
                i = 2 * i + 1;
            else
                i = 2 * i + 2;
        }
    }
};
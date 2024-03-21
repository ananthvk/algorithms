#pragma once
#include <deque>

template <typename T, class Container = std::deque<T>> class Stack
{
  private:
    Container c;

  public:
    typedef Container container_type;
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    Stack() {}

    const_reference top() const { return c.back(); }

    bool empty() const { return c.empty(); }

    size_type size() const { return c.size(); }

    void push(const T &t) { c.push_back(t); }

    void push(T &&t) { c.push_back(std::move(t)); }

    void pop() { c.pop_back(); }
};
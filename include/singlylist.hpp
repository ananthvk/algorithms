#pragma once
#include <iostream>
#include <ostream>

template <typename T> class SinglyLinkedList
{
  private:
    class Node
    {
        T value;
        Node *next;
        friend SinglyLinkedList;
    };

    Node *head;
    Node *tail;
    size_t sz;


  public:
    typedef size_t size_type;
    typedef T &reference;
    typedef const T &const_reference;
    typedef T value_type;

    class ConstIterator
    {
        Node *node;

      public:
        ConstIterator(Node *node) : node(node) {}

        // Define a forward iterator
        const T &operator*() const { return node->value; }

        // Post increment
        ConstIterator operator++(int)
        {
            ConstIterator it(node);
            node = node->next;
            return it;
        }

        // Pre increment
        ConstIterator &operator++()
        {
            node = node->next;
            return *this;
        }

        bool operator==(const ConstIterator &other) const { return node == other.node; }

        bool operator!=(const ConstIterator &other) const { return !(*this == other); }
    };

    class Iterator
    {
        Node *node;

      public:
        Iterator(Node *node) : node(node) {}

        // Define a forward iterator
        T &operator*() const { return node->value; }

        // Post increment
        Iterator operator++(int)
        {
            Iterator it(node);
            node = node->next;
            return it;
        }

        // Pre increment
        Iterator &operator++()
        {
            node = node->next;
            return *this;
        }

        operator ConstIterator() const { return ConstIterator(node); }

        bool operator==(const Iterator &other) const { return node == other.node; }

        bool operator!=(const Iterator &other) const { return !(*this == other); }
    };

    SinglyLinkedList() : head(nullptr), tail(nullptr), sz(0) {}

    Iterator begin() { return Iterator(head); }

    Iterator end() { return Iterator(nullptr); }

    ConstIterator begin() const { return ConstIterator(head); }

    ConstIterator end() const { return ConstIterator(nullptr); }

    // Prepends the value to the front of the list
    void push_front(const T &t)
    {
        Node *new_node = new Node;
        new_node->value = t;
        // The list is currently empty
        if (head == nullptr)
        {
            new_node->next = nullptr;
            head = new_node;
            tail = new_node;
        }
        else
        {
            new_node->next = head;
            head = new_node;
        }
        ++sz;
    }

    // Appends the value to the end of the list
    void push_back(const T &t)
    {
        Node *new_node = new Node;
        new_node->value = t;
        new_node->next = nullptr;

        // The list is currently empty
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
        ++sz;
    }

    size_t size() const { return sz; }

    void clear()
    {

        while (head)
        {
            Node *next_node = head->next;
            delete head;
            head = next_node;
        }
        sz = 0;
        head = nullptr;
        tail = nullptr;
    }

    ~SinglyLinkedList() { clear(); }

    // Rule of 5
    // Copy constructor, move constructor, copy assignment, move assignment, and the destructor

    SinglyLinkedList(const SinglyLinkedList &other)
    {
        for (const auto &v : other)
        {
            push_back(v);
        }
    }

    SinglyLinkedList &operator=(SinglyLinkedList other)
    {
        swap(*this, other);
        return *this;
    }

    friend void swap(SinglyLinkedList &first, SinglyLinkedList &second)
    {
        using std::swap;
        swap(first.head, second.head);
        swap(first.tail, second.tail);
        swap(first.sz, second.sz);
    }

    SinglyLinkedList(SinglyLinkedList &&other) noexcept { swap(*this, other); }

    // Solve problems from
    // http://cslibrary.stanford.edu/105/LinkedListProblems.pdf
    // ===========================================================
    Iterator get_nth(size_t n)
    {
        if (sz == 0 || n >= sz)
        {
            throw std::out_of_range("Index out of range for list");
        }
        Node *h = head;
        for (size_t i = 0; i < n; i++)
        {
            h = h->next;
        }
        return Iterator(h);
    }

    bool empty() const { return sz == 0; }

    T &back() { return tail->value; }

    const T &back() const { return tail->value; }
};
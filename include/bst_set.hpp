#pragma once
#include <limits>
#include <stdexcept>

// Some differences w.r.t standard library set
// 1. Decrementing begin() results in end() and not undefined behavior

template <typename Key, class Compare = std::less<Key>> class BSTSet
{
  private:
    class Node
    {
        Node *right;
        Node *left;
        Node *parent;
        Key value;
        friend BSTSet;

        Node() : right(nullptr), left(nullptr), parent(nullptr) {}

        Node(const Key &value) : right(nullptr), left(nullptr), parent(nullptr), value(value) {}
    };

    Node *root;
    Node *leftmost;
    Node *rightmost;
    size_t sz;

    void clear_(Node *n)
    {
        if (n)
        {
            clear_(n->left);
            clear_(n->right);
            delete n;
        }
    }

    Node *get_node(const Key &t)
    {
        sz++;
        return new Node(t);
    }


  public:
    typedef Key value_type;
    typedef Key key_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
    typedef Key &reference;
    typedef const Key &const_reference;
    class Iterator;
    typedef Iterator iterator;
    typedef const Iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    class Iterator : public std::iterator<std::bidirectional_iterator_tag, Key>
    {
      private:
        Node *node;
        // Holds the address of the rightmost pointer in the parent container
        Node *const *rightmost_node;

        void prev()
        {
            // Moves this node to the previous node which is lesser than this node
            if (!node)
            {
                node = *rightmost_node;
                return;
            }
            Node *inorder_pred = node->left, *current, *parent;
            // If the node has a left sub child, traverse the right subtree of the left subchild
            if (node->left)
            {
                inorder_pred = node->left;
                while (inorder_pred && inorder_pred->right)
                    inorder_pred = inorder_pred->right;
                node = inorder_pred;
                return;
            }

            // If the node has a parent,
            if (node->parent)
            {
                current = node;
                parent = node->parent;
                // Keep traversing up, until we reach a node which is the right subtree for it's
                // parent
                while (parent && parent->left == current)
                {
                    current = parent;
                    parent = current->parent;
                }
                node = parent;
            }

            else
            {
                // This is the root node, which does not have a left subtree
                node = nullptr;
            }
        }

        void next()
        {
            // Move node to next node which is greater than this node
            if (!node)
            {
                throw std::out_of_range("Out of range");
            }
            Node *inorder_successor = node->right, *current, *parent;

            // If this node has a right subtree, find the inorder sucessor
            if (node->right)
            {
                inorder_successor = node->right;
                while (inorder_successor && inorder_successor->left)
                    inorder_successor = inorder_successor->left;
                node = inorder_successor;
                return;
            }

            // If the node has a parent,
            if (node->parent)
            {
                current = node;
                parent = node->parent;
                // Keep traversing up, until we reach a node who is the left subtree for it's parent
                while (parent && parent->right == current)
                {
                    current = parent;
                    parent = current->parent;
                }
                node = parent;
            }

            else
            {
                // This is the root node, which does not have a right subtree
                node = nullptr;
            }
        }


      public:
        typedef Key value_type;
        typedef const Key &reference;
        typedef const Key *pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

        Iterator() : node(nullptr), rightmost_node(nullptr) {}

        Iterator(Node *node, Node *const *rightmost_node)
            : node(node), rightmost_node(rightmost_node)
        {
        }

        bool operator==(const Iterator &other) const { return node == other.node; }

        bool operator!=(const Iterator &other) const { return !(*this == other); }

        // Post increment
        Iterator operator++(int)
        {
            Iterator it(node, rightmost_node);
            next();
            return it;
        }

        // Pre-increment
        Iterator &operator++()
        {
            next();
            return *this;
        }

        // Post decrement
        Iterator operator--(int)
        {
            Iterator it(node, rightmost_node);
            prev();
            return it;
        }

        // Pre decrement
        Iterator &operator--()
        {
            prev();
            return *this;
        }

        const Key &operator*() const { return node->value; }
    };

    BSTSet() : root(nullptr), leftmost(nullptr), rightmost(nullptr), sz(0) {}

    size_t size() { return sz; }

    std::pair<Iterator, bool> insert(const Key &t)
    {
        Node *current = root, *parent = nullptr, *new_node;
        if (!root)
        {
            root = get_node(t);
            leftmost = root;
            rightmost = root;
            return std::make_pair(Iterator(root, &rightmost), true);
        }
        if (t == leftmost->value)
        {
            return std::make_pair(Iterator(leftmost, &rightmost), false);
        }
        if (t == rightmost->value)
        {
            return std::make_pair(Iterator(rightmost, &rightmost), false);
        }
        if (Compare{}(t, leftmost->value))
        {
            leftmost->left = get_node(t);
            leftmost->left->parent = leftmost;
            leftmost = leftmost->left;
            return std::make_pair(Iterator(leftmost, &rightmost), true);
        }
        if (!Compare{}(t, rightmost->value))
        {
            rightmost->right = get_node(t);
            rightmost->right->parent = rightmost;
            rightmost = rightmost->right;
            return std::make_pair(Iterator(rightmost, &rightmost), true);
        }
        while (current)
        {
            parent = current;
            if (Compare{}(t, current->value))
                current = current->left;
            else if (t == current->value)
                break;
            else
                current = current->right;
        }
        if (current)
        {
            return std::make_pair(Iterator(current, &rightmost), false);
        }
        new_node = get_node(t);
        new_node->parent = parent;
        if (Compare{}(t, parent->value))
            parent->left = new_node;
        else
            parent->right = new_node;
        return std::make_pair(Iterator(new_node, &rightmost), true);
    }

    void clear()
    {
        // TODO: Make an iterative clear function instead
        clear_(root);
        root = nullptr;
    }

    iterator begin() const { return Iterator(leftmost, &rightmost); }

    iterator end() const { return Iterator(nullptr, &rightmost); }

    const_iterator cbegin() const { return begin(); }

    const_iterator cend() const { return end(); }

    reverse_iterator rbegin() const { return reverse_iterator(end()); }

    reverse_iterator rend() const { return reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }

    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

    iterator find(const Key &x)
    {
        if (!root)
            return end();
        if (x > rightmost->value)
            return end();
        if (x < leftmost->value)
            return end();
        auto current = root;
        while (current)
        {
            if (x < current->value)
                current = current->left;
            else if (x > current->value)
                current = current->right;
            else
                return Iterator(current, &rightmost);
        }
        return end();
    }

    // TODO: Implement erase

    ~BSTSet() { clear(); }

    bool empty() const { if(x){return sz == 0; }
    
    size_type max_size() const { return std::numeric_limits<difference_type>::max(); }

    // TODO: Implement rule of 5
    // TODO: Implement iterative clear function
};

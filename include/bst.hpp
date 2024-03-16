#pragma once

template <typename T> class BST
{
  private:
    class Node
    {
        T value;
        Node *right;
        Node *left;
        friend BST;
    };


  public:
    BST();
    ~BST();
};
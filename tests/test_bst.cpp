#include "bst_set.hpp"
#include "gtest/gtest.h"
#include <set>
#include <vector>

TEST(BST, TestEmpty)
{
    BSTSet<int> s;
    ASSERT_EQ(s.size(), 0);
}

TEST(BST, TestInsertion)
{
    BSTSet<int> s;
    auto it1 = s.insert(3);
    auto it2 = s.insert(2);
    auto it3 = s.insert(5);
    ASSERT_EQ(*it1.first, 3);
    ASSERT_EQ(*it2.first, 2);
    ASSERT_EQ(*it3.first, 5);
    ASSERT_EQ(s.size(), 3);
}

TEST(BST, TestInsertionSameValues)
{
    BSTSet<int> s;
    auto it = s.insert(5);
    ASSERT_EQ(*it.first, 5);
    ASSERT_EQ(it.second, true);
    s.insert(3);
    s.insert(6);
    s.insert(2);
    s.insert(8);
    ASSERT_EQ(s.size(), 5);

    auto it2 = s.insert(5);
    ASSERT_EQ(*it2.first, 5);
    ASSERT_EQ(it2.second, false);
    ASSERT_EQ(it.first, it2.first);
}

TEST(BST, TestInsertionSingleValue)
{
    BSTSet<int> s;
    auto it = s.insert(5);
    auto it1 = s.insert(5);
    auto it2 = s.insert(5);
    auto it3 = s.insert(5);

    ASSERT_EQ(it.first, it1.first);
    ASSERT_EQ(it1.first, it2.first);
    ASSERT_EQ(it2.first, it3.first);
    ASSERT_EQ(it1.second, false);
    ASSERT_EQ(it2.second, false);
    ASSERT_EQ(it3.second, false);
    ASSERT_EQ(s.size(), 1);
}

TEST(BST, TestInsertionSmallest)
{
    BSTSet<int> s;
    auto it1 = s.insert(5).first;
    auto it2 = s.insert(4).first;
    auto it3 = s.insert(3).first;
    auto it4 = s.insert(2).first;
    auto it5 = s.insert(1).first;
    it5++;
    it4++;
    it3++;
    it2++;
    it1++;
    ASSERT_EQ(*it5, 2);
    ASSERT_EQ(*it4, 3);
    ASSERT_EQ(*it3, 4);
    ASSERT_EQ(*it2, 5);
    ASSERT_EQ(it1, s.end());
    ASSERT_EQ(s.size(), 5);
}

TEST(BST, TestInsertionLargest)
{
    BSTSet<int> s;
    auto it1 = s.insert(1).first;
    auto it2 = s.insert(2).first;
    auto it3 = s.insert(3).first;
    auto it4 = s.insert(4).first;
    auto it5 = s.insert(5).first;
    it5++;
    it4++;
    it3++;
    it2++;
    it1++;
    ASSERT_EQ(it5, s.end());
    ASSERT_EQ(*it4, 5);
    ASSERT_EQ(*it3, 4);
    ASSERT_EQ(*it2, 3);
    ASSERT_EQ(*it1, 2);
    ASSERT_EQ(s.size(), 5);
}

TEST(BST, TestInorderSuccessorRightSubtree)
{
    BSTSet<int> s;
    auto it = s.insert(30).first;
    auto it2 = it;
    s.insert(20);
    s.insert(50);
    it++;
    ASSERT_EQ(*it, 50);

    s.insert(40);
    s.insert(38);
    s.insert(34);
    s.insert(36);
    it2++;
    ASSERT_EQ(*it2, 34);

    s.clear();
    it = s.insert(100).first;
    s.insert(110);
    s.insert(120);
    s.insert(130);
    it++;

    ASSERT_EQ(*it, 110);
}

TEST(BST, TestInorderSuccessorNoRightSubtree)
{
    BSTSet<int> s;
    auto it = s.insert(32).first;
    auto it1 = s.insert(16).first;
    s.insert(48);

    it1++;
    ASSERT_EQ(*it, 32);
    ASSERT_EQ(it1, it);

    s.clear();
    s.insert(60);
    s.insert(45);
    s.insert(50);
    it = s.insert(55).first;
    it++;
    ASSERT_EQ(*it, 60);
    it++;
    ASSERT_EQ(it, s.end());

    s.clear();
    s.insert(60);
    s.insert(45);
    s.insert(50);
    it = s.insert(48).first;
    it++;
    ASSERT_EQ(*it, 50);
    std::set<int> sss;
}

TEST(BST, TestLargeNumberOfInsertions)
{
    BSTSet<int> s;
    std::set<int> stdset;
    std::vector<int> v = {1, 8, 21, 23, 45, 64, 32, 12, 19, 24};
    for (int i = 0; i < 100; i++)
    {
        if (i % 2)
        {
            s.insert(i + 5000);
            stdset.insert(i + 5000);
        }
        else
        {
            s.insert(i - 5000);
            stdset.insert(i - 5000);
        }
    }
    ASSERT_EQ(s.size(), 100);
    ASSERT_EQ(*s.begin(), -5000);

    for (const auto &e : v)
    {
        s.insert(e);
        stdset.insert(e);
    }

    auto s_it = s.begin();
    auto stdset_it = stdset.begin();
    for (int i = 0; i < 100 + (int)v.size(); i++)
    {
        ASSERT_EQ(*s_it, *stdset_it);
        s_it++;
        stdset_it++;
    }
}

TEST(BST, RangeBasedForLoop)
{
    BSTSet<int> s;
    s.insert(3);
    s.insert(8);
    s.insert(2);
    s.insert(7);
    s.insert(-1);
    s.insert(16);
    s.insert(0);
    std::vector<int> v;
    std::vector<int> v1 = {-1, 0, 2, 3, 7, 8, 16};
    for (const auto &e : s)
    {
        v.push_back(e);
    }
    ASSERT_EQ(v.size(), s.size());
    ASSERT_EQ(v1, v);
}

TEST(BST, TestInorderPredecessorLeftSubtree)
{
    BSTSet<int> s;
    auto it = s.insert(30).first;
    auto it2 = it;
    s.insert(20);
    s.insert(50);
    it--;
    ASSERT_EQ(*it, 20);

    s.insert(22);
    s.insert(23);
    s.insert(24);
    s.insert(28);
    s.insert(40);
    s.insert(38);
    s.insert(34);
    s.insert(36);
    it2--;
    ASSERT_EQ(*it2, 28);

    s.clear();
    it = s.insert(100).first;
    s.insert(90);
    s.insert(80);
    s.insert(70);
    it--;

    ASSERT_EQ(*it, 90);
}

TEST(BST, TestInorderPredecessorNoLeftSubtree)
{
    BSTSet<int> s;
    auto it = s.insert(32).first;
    s.insert(16);
    auto it1 = s.insert(48).first;

    it1--;
    ASSERT_EQ(*it, 32);
    ASSERT_EQ(it1, it);

    s.clear();
    s.insert(60);
    s.insert(45);
    s.insert(50);
    it = s.insert(65).first;
    it--;
    ASSERT_EQ(*it, 60);
    it--;
    ASSERT_EQ(*it, 50);
    it--;
    ASSERT_EQ(*it, 45);
    it--;
    ASSERT_EQ(it, s.end());
}

TEST(BST, TestReverseIterator)
{
    BSTSet<int> s;
    std::set<int> ss;

    std::vector<int> elements = {1, 8, 3, 2, 7, 6, 4, 0, -1, 5, 20, 24, 19, -20};
    for (const auto &e : elements)
    {
        s.insert(e);
        ss.insert(e);
    }
    BSTSet<int>::reverse_iterator it = s.rbegin();
    std::set<int>::reverse_iterator it1 = ss.rbegin();
    while (it != s.rend() && it1 != ss.rend())
    {
        ASSERT_EQ(*it, *it1);
        it++;
        it1++;
    }
}

TEST(BST, TestFind)
{
    BSTSet<int> s;
    std::vector<int> elements = {1, 8, 3, 7, 6, 4, 0, -1, 5, 20, 24, 19, -20};
    for (const auto &e : elements)
    {
        s.insert(e);
    }
    ASSERT_EQ(*s.find(8), 8);
    ASSERT_EQ(s.find(-2), s.end());
    ASSERT_EQ(s.find(2), s.end());
    ASSERT_EQ(s.find(-1000), s.end());
    ASSERT_EQ(s.find(1000), s.end());
    ASSERT_EQ(s.find(-21), s.end());
    ASSERT_EQ(s.find(25), s.end());
    ASSERT_EQ(s.find(9), s.end());
    for (const auto &e : elements)
    {
        ASSERT_EQ(*s.find(e), e);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
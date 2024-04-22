#include "priority_queue.hpp"
#include "gtest/gtest.h"

TEST(PriorityQ, Empty)
{
    PriorityQueue<int> pq;
    ASSERT_EQ(pq.size(), 0);
    ASSERT_EQ(pq.empty(), true);
}

TEST(PriorityQ, Push)
{
    PriorityQueue<int> pq;
    pq.push(3);
    pq.push(8);
    ASSERT_EQ(pq.size(), 2);
    ASSERT_EQ(pq.empty(), false);
}

TEST(PriorityQ, Top)
{
    PriorityQueue<int> pq;
    pq.push(3);
    ASSERT_EQ(pq.top(), 3);
    pq.push(8);
    ASSERT_EQ(pq.top(), 8);
    pq.push(12);
    ASSERT_EQ(pq.top(), 12);
    pq.push(16);
    ASSERT_EQ(pq.top(), 16);
    pq.push(26);
    ASSERT_EQ(pq.top(), 26);

    pq.push(21);
    ASSERT_EQ(pq.top(), 26);
    pq.push(27);
    ASSERT_EQ(pq.top(), 27);
    pq.push(22);
    ASSERT_EQ(pq.top(), 27);
    pq.push(32);
    ASSERT_EQ(pq.top(), 32);
    pq.push(0);
    pq.push(1);
    pq.push(18);
    pq.push(31);
    pq.push(29);
    ASSERT_EQ(pq.top(), 32);
}

TEST(PriorityQ, Pop)
{
    PriorityQueue<int> pq;
    pq.push(1);
    pq.push(2);
    pq.push(3);
    pq.push(4);
    pq.push(5);
    pq.push(-1);
    pq.push(-2);
    pq.push(-3);
    pq.push(-4);
    pq.push(-5);
    pq.push(10);
    pq.push(6);
    pq.push(7);
    pq.push(8);
    pq.push(9);

    ASSERT_EQ(pq.top(), 10);
    pq.pop();
    ASSERT_EQ(pq.top(), 9);
    pq.pop();
    ASSERT_EQ(pq.top(), 8);
    pq.pop();
    ASSERT_EQ(pq.top(), 7);
    pq.pop();
    ASSERT_EQ(pq.top(), 6);
    pq.pop();
    ASSERT_EQ(pq.top(), 5);
    pq.pop();
    ASSERT_EQ(pq.top(), 4);
    pq.pop();
    ASSERT_EQ(pq.top(), 3);
    pq.pop();
    ASSERT_EQ(pq.top(), 2);
    pq.pop();
    ASSERT_EQ(pq.top(), 1);
    pq.pop();
    ASSERT_EQ(pq.top(), -1);
    pq.pop();
    ASSERT_EQ(pq.top(), -2);
    pq.pop();
    ASSERT_EQ(pq.top(), -3);
    pq.pop();
    ASSERT_EQ(pq.top(), -4);
    pq.pop();
    ASSERT_EQ(pq.top(), -5);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);
    ASSERT_EQ(pq.empty(), true);
}

TEST(PriorityQ, Loop)
{
    std::vector<int> result;
    PriorityQueue<int> pq;
    for (int i = 15; i > 0; i--)
        pq.push(i);
    ASSERT_EQ(pq.top(), 15);
    while (!pq.empty())
    {
        result.push_back(pq.top());
        pq.pop();
    }
    for (int i = 0; i < 14; i++)
        ASSERT_GE(result[i], result[i + 1]);
}

TEST(PriorityQ, MinHeap)
{
    std::vector<int> result;
    PriorityQueue<int, std::vector<int>, std::greater<int>> pq;
    for (int i = 0; i < 15; i++)
        pq.push(i);
    ASSERT_EQ(pq.top(), 0);

    while (!pq.empty())
    {
        result.push_back(pq.top());
        pq.pop();
    }
    for (int i = 0; i < 14; i++)
        ASSERT_LE(result[i], result[i + 1]);
}

TEST(PriorityQ, MiscOperations)
{
    std::vector<int> result;
    PriorityQueue<int> pq;

    pq.push(8);
    ASSERT_EQ(pq.top(), 8);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);

    pq.push(3);
    pq.push(4);
    ASSERT_EQ(pq.size(), 2);
    ASSERT_EQ(pq.top(), 4);
    pq.pop();
    ASSERT_EQ(pq.top(), 3);
    ASSERT_EQ(pq.size(), 1);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);

    pq.push(4);
    pq.push(3);
    ASSERT_EQ(pq.size(), 2);
    ASSERT_EQ(pq.top(), 4);
    pq.pop();
    ASSERT_EQ(pq.top(), 3);
    ASSERT_EQ(pq.size(), 1);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);

    pq.push(3);
    pq.push(4);
    pq.push(5);
    ASSERT_EQ(pq.top(), 5);
    pq.pop();
    ASSERT_EQ(pq.top(), 4);
    pq.pop();
    ASSERT_EQ(pq.top(), 3);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);


    pq.push(1);
    pq.push(2);
    pq.push(3);

    ASSERT_EQ(pq.top(), 3);
    pq.pop();
    ASSERT_EQ(pq.top(), 2);
    pq.pop();
    ASSERT_EQ(pq.top(), 1);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);

    pq.push(1);
    pq.push(3);
    pq.push(2);

    ASSERT_EQ(pq.top(), 3);
    pq.pop();
    ASSERT_EQ(pq.top(), 2);
    pq.pop();
    ASSERT_EQ(pq.top(), 1);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);

    pq.push(2);
    pq.push(1);
    pq.push(3);

    ASSERT_EQ(pq.top(), 3);
    pq.pop();
    ASSERT_EQ(pq.top(), 2);
    pq.pop();
    ASSERT_EQ(pq.top(), 1);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);

    pq.push(2);
    pq.push(3);
    pq.push(1);

    ASSERT_EQ(pq.top(), 3);
    pq.pop();
    ASSERT_EQ(pq.top(), 2);
    pq.pop();
    ASSERT_EQ(pq.top(), 1);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);

    pq.push(3);
    pq.push(1);
    pq.push(2);

    ASSERT_EQ(pq.top(), 3);
    pq.pop();
    ASSERT_EQ(pq.top(), 2);
    pq.pop();
    ASSERT_EQ(pq.top(), 1);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);

    pq.push(3);
    pq.push(2);
    pq.push(1);

    ASSERT_EQ(pq.top(), 3);
    pq.pop();
    ASSERT_EQ(pq.top(), 2);
    pq.pop();
    ASSERT_EQ(pq.top(), 1);
    pq.pop();
    ASSERT_EQ(pq.size(), 0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
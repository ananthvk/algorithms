#include "singlylist.hpp"
#include "gtest/gtest.h"


TEST(SinglyList, TestEmpty)
{
    SinglyLinkedList<int> s;
    ASSERT_EQ(s.begin(), SinglyLinkedList<int>::Iterator(nullptr));
    ASSERT_EQ(s.end(), SinglyLinkedList<int>::Iterator(nullptr));
    ASSERT_EQ(s.size(), 0);
}

TEST(SinglyList, TestIterators)
{
    SinglyLinkedList<int> s;
    s.push_back(5);
    ASSERT_EQ(s.size() ,  1);
    ASSERT_EQ(*s.begin() ,  5);
    ASSERT_EQ(s.end() ,  SinglyLinkedList<int>::Iterator(nullptr));

    s.push_back(10);
    s.push_back(15);
    s.push_back(20);
    s.push_back(25);

    ASSERT_EQ(s.size() ,  5);
    ASSERT_EQ(*s.begin() ,  5);

    auto it = s.begin();
    ASSERT_EQ(*it++ ,  5);
    ASSERT_EQ(*it++ ,  10);
    ASSERT_EQ(*it++ ,  15);
    ASSERT_EQ(*it++ ,  20);
    ASSERT_EQ(*it++ ,  25);
    ASSERT_EQ(it ,  s.end());

    SinglyLinkedList<int> s2;
    s2.push_front(8);
    ASSERT_EQ(s2.size() ,  1);
    ASSERT_EQ(*s2.begin() ,  8);
    ASSERT_EQ(s2.end() ,  SinglyLinkedList<int>::Iterator(nullptr));

    s2.push_front(9);
    s2.push_front(10);
    s2.push_front(11);
    s2.push_front(12);

    it = s2.begin();
    ASSERT_EQ(*it++ ,  12);
    ASSERT_EQ(*it++ ,  11);
    ASSERT_EQ(*it++ ,  10);
    ASSERT_EQ(*it++ ,  9);
    ASSERT_EQ(*it++ ,  8);
    ASSERT_EQ(it ,  s2.end());

    // Test intertwining of both push back and push front operations
    SinglyLinkedList<int> s3;
    s3.push_back(0);

    s3.push_back(1);
    s3.push_back(2);
    s3.push_front(-1);
    s3.push_front(-2);

    it = s3.begin();
    ASSERT_EQ(*it++ ,  -2);
    ASSERT_EQ(*it++ ,  -1);
    ASSERT_EQ(*it++ ,  0);
    ASSERT_EQ(*it++ ,  1);
    ASSERT_EQ(*it++ ,  2);
    ASSERT_EQ(s3.size() ,  5);
}

TEST(SinglyList, TestRangeForLoops)
{
    SinglyLinkedList<int> sll;
    std::vector<int> vec;
    std::vector<int> reference = {1, 3, 5, 7, 9, 11, 13, 15};
    for (const auto &i : reference)
    {
        sll.push_back(i);
    }
    for (const auto &v : sll)
    {
        vec.push_back(v);
    }
    ASSERT_EQ(vec ,  reference);
}

/*
TEST("Test that copy constructors and assignments work", "[singlylist]") {}

TEST("Test that move constructors and assignments work", "[singlylist]") {}

TEST("Test const iterator conversion works", "[singlylist]") {}
*/

TEST(SinglyList, GetNthElement)
{
    SinglyLinkedList<int> sll;
    EXPECT_THROW(sll.get_nth(0), std::out_of_range);
    EXPECT_THROW(sll.get_nth(1), std::out_of_range);
    EXPECT_THROW(sll.get_nth(5), std::out_of_range);
    sll.push_back(5);
    ASSERT_EQ(sll.get_nth(0) ,  sll.begin());

    sll.push_back(4);
    sll.push_back(3);
    sll.push_back(2);
    sll.push_back(1);

    auto it = sll.begin();
    ASSERT_EQ(sll.get_nth(1) ,  ++it);
    ASSERT_EQ(sll.get_nth(2) ,  ++it);
    ASSERT_EQ(sll.get_nth(3) ,  ++it);
    ASSERT_EQ(sll.get_nth(4) ,  ++it);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
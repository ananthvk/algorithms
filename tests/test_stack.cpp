#include "stack.hpp"
#include "gtest/gtest.h"

#include<stack>
TEST(Stack, Empty)
{
    Stack<int, std::vector<int>> s;
    s.push(3);
    s.push(8);
    s.push(12);
    s.push(14);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
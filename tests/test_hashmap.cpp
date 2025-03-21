#include "hashmap.hpp"
#include "gtest/gtest.h"

TEST(HashMapTest, Empty)
{
    HashMap<std::string, int> h;
    ASSERT_EQ(h.size(), 0);
    ASSERT_EQ(h.load_factor(), 0);
    ASSERT_NEAR(h.max_load_factor(), h.DEFAULT_MAX_LOAD_FACTOR, 1e-5);
}

TEST(HashMapTest, ModifyLoadFactor)
{
    HashMap<std::string, int> h;
    h.max_load_factor(0.3f);
    ASSERT_NEAR(h.max_load_factor(), 0.3, 1e-5);
    h.max_load_factor(0.5f);
    ASSERT_NEAR(h.max_load_factor(), 0.5, 1e-5);
    ASSERT_EQ(h.load_factor(), 0);
}

TEST(HashMapTest, InsertAndRetrieveValues)
{
    HashMap<std::string, int> h;
    h.insert("Hello there", 58);
    h.insert("Who are you?", 31);
    h.insert("C++", 198339);
    h.insert("XYZ", -88881);
    ASSERT_EQ(h.size(), 4);
    ASSERT_EQ(h.find("Hello there"), 58);
    ASSERT_EQ(h.find("Who are you?"), 31);
    ASSERT_EQ(h.find("C++"), 198339);
    ASSERT_EQ(h.find("XYZ"), -88881);
    ASSERT_EQ(h.size(), 4);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
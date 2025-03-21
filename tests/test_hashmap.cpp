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

TEST(HashMapTest, LargeNumberOfValues)
{
    HashMap<int, long long int> h;
    for (int i = 0; i < 780000; i++)
        h.insert(i, static_cast<long long int>(i) * i);
    ASSERT_EQ(h.size(), 780000);
    for (int i = 0; i < 780000; i++)
        ASSERT_EQ(h.find(i), static_cast<long long int>(i) * i);
}

TEST(HashMapTest, ElementsThatAreNotPresent)
{
    HashMap<std::string, int> h;
    h.insert("Hello there", 58);
    h.insert("Who are you?", 31);
    ASSERT_EQ(h.find("SDFSDF"), std::nullopt);
    ASSERT_EQ(h.find(""), std::nullopt);
    ASSERT_EQ(h.find("Hello there "), std::nullopt);
}

TEST(HashMapTest, UpdateOperation)
{
    HashMap<int, long long int> h;
    for (int i = 0; i < 5000; i++)
        h.insert(i, static_cast<long long int>(i) * i);
    for (int i = 0; i < 5000; i++)
        h.insert(i, static_cast<long long int>(i) * i * i);
    for (int i = 0; i < 5000; i++)
        ASSERT_EQ(h.find(i), static_cast<long long int>(i) * i * i);
    
    HashMap<std::string, std::string> kv;
    kv.insert("Hello", "World");
    kv.insert("C+", "+");
    kv.insert("Empty", "");
    ASSERT_EQ(kv.find("Hello"), "World");
    ASSERT_EQ(kv.find("C+"), "+");
    ASSERT_EQ(kv.find("Empty"), "");

    kv.insert("Hello", "World New");
    kv.insert("C+", "New +");
    kv.insert("Empty", "Not anymore");
    ASSERT_EQ(kv.find("Hello"), "World New");
    ASSERT_EQ(kv.find("C+"), "New +");
    ASSERT_EQ(kv.find("Empty"), "Not anymore");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
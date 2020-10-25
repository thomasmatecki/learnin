#include <iostream>
#include <gtest/gtest.h>
#include "array_list.cpp"


TEST(ArrayListSuite, SimpleCases)
{
    ArrayList<char> a = ArrayList<char>();
    char c = 'c';
    a.append(&c);
    EXPECT_EQ(1,2);
}
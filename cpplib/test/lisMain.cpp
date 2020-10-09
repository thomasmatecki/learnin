#include <iostream>
#include <gtest/gtest.h>

int lis(int, int[]);

TEST(LongestIncreasingSubSequenceTestSuite, LISBaseCases)
{
    const int LENGTH = 1;
    int in[LENGTH] = {1};
    EXPECT_EQ(lis(LENGTH, in), 1) << "The value is not correct";
}

TEST(LongestIncreasingSubSequenceTestSuite, LISSimpleIncreasing)
{
    const int LENGTH0 = 12;
    int in0[LENGTH0] = {5, 7, 4, -3, 9, 1, 10, 4, 5, 8, 9, 3};
    // -3, 1, 4, 5, 8 9
    EXPECT_EQ(lis(LENGTH0, in0), 6) << "The value is not correct";
}
TEST(LongestIncreasingSubSequenceTestSuite, LISAllIncreasing)
{
    const int LENGTH1 = 6;
    int in1[LENGTH1] = {5, 7, 8, 9, 10, 11};
    EXPECT_EQ(lis(LENGTH1, in1), 6) << "The value is not correct";
}
TEST(LongestIncreasingSubSequenceTestSuite, LISDuplicateEnd)
{
    const int LENGTH2 = 6;
    int in2[LENGTH2] = {5, 7, 8, 9, 10, 10};
    EXPECT_EQ(lis(LENGTH2, in2), 5) << "The value is not correct";
}
TEST(LongestIncreasingSubSequenceTestSuite, LISDumplicateStart)
{
    const int LENGTH3 = 6;
    int in3[LENGTH3] = {5, 5, 7, 8, 9, 10};
    EXPECT_EQ(lis(LENGTH3, in3), 5) << "The value is not correct";
}
TEST(LongestIncreasingSubSequenceTestSuite, LISAllDescreasing)
{
    const int LENGTH4 = 6;
    int in4[LENGTH4] = {10, 9, 8, 7, 6, 5};
    EXPECT_EQ(lis(LENGTH4, in4), 1) << "The value is not correct";
}
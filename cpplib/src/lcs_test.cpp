#include <iostream>
#include <gtest/gtest.h>

int lcs(char[], char[]);
TEST(LongestCommonSubsequenceTestSuite,
     LCSBaseCases)
{
    char in0[] = "ABC";
    char in1[] = "ABC";
    EXPECT_EQ(lcs(in0, in1), 3) << "The value is not correct";

    char in2[] = "A";
    char in3[] = "A";
    EXPECT_EQ(lcs(in2, in3), 1) << "The value is not correct";
}

TEST(LongestCommonSubsequenceTestSuite,
     LCSDifferingLengthCases)
{
    char in0[] = "AABC";
    char in1[] = "ABC";
    EXPECT_EQ(lcs(in0, in1), 3) << "The value is not correct";

    char in2[] = "ABC";
    char in3[] = "ABCC";
    EXPECT_EQ(lcs(in2, in3), 3) << "The value is not correct";

    char in4[] = "AABCC";
    char in5[] = "ABC";
    EXPECT_EQ(lcs(in4, in5), 3) << "The value is not correct";
}
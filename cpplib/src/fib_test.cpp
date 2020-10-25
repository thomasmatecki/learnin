#include <iostream>
#include <gtest/gtest.h>

int fib1(int);

TEST(Fibonacci1TestSuite, FibonacciBaseCases)
{
    EXPECT_EQ(fib1(0), 0) << "The value is not correct";
    EXPECT_EQ(fib1(1), 1) << "The value is not correct";
}

TEST(Fibonacci1TestSuite, FibonacciRecurrent)
{
    EXPECT_EQ(fib1(2), 1) << "The value is not correct";
    EXPECT_EQ(fib1(3), 2) << "The value is not correct";
    EXPECT_EQ(fib1(4), 3) << "The value is not correct";
    EXPECT_EQ(fib1(5), 5) << "The value is not correct";
    EXPECT_EQ(fib1(6), 8) << "The value is not correct";
}

TEST(Fibonacci2TestSuite, FibonacciBaseCases)
{
    EXPECT_EQ(fib1(0), 0) << "The value is not correct";
    EXPECT_EQ(fib1(1), 1) << "The value is not correct";
}

TEST(Fibonacci2TestSuite, FibonacciRecurrent)
{
    EXPECT_EQ(fib1(2), 1) << "The value is not correct";
    EXPECT_EQ(fib1(3), 2) << "The value is not correct";
    EXPECT_EQ(fib1(4), 3) << "The value is not correct";
    EXPECT_EQ(fib1(5), 5) << "The value is not correct";
    EXPECT_EQ(fib1(6), 8) << "The value is not correct";
    EXPECT_EQ(fib1(7), 13) << "The value is not correct";
    EXPECT_EQ(fib1(8), 21) << "The value is not correct";
}

TEST(Fibonacci3TestSuite, FibonacciBaseCases)
{
    EXPECT_EQ(fib1(0), 0) << "The value is not correct";
    EXPECT_EQ(fib1(1), 1) << "The value is not correct";
}

TEST(Fibonacci3TestSuite, FibonacciRecurrent)
{
    EXPECT_EQ(fib1(2), 1) << "The value is not correct";
    EXPECT_EQ(fib1(3), 2) << "The value is not correct";
    EXPECT_EQ(fib1(4), 3) << "The value is not correct";
    EXPECT_EQ(fib1(5), 5) << "The value is not correct";
    EXPECT_EQ(fib1(6), 8) << "The value is not correct";
    EXPECT_EQ(fib1(7), 13) << "The value is not correct";
    EXPECT_EQ(fib1(8), 21) << "The value is not correct";
}
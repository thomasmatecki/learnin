#include <assert.h>

int fib1(int n)
{
    assert(n >= 0);
    if (n == 0)
    {
        return 0;
    }
    else if (n == 1)
    {
        return 1;
    }
    else
    {
        return fib1(n - 1) + fib1(n - 2);
    }
}
/*
 * F[i] is the i'th Fibonacci number.
 * F[i] = F[i-1] + F[i-2]
 */
int fib2(int n)
{
    int a[n];
    a[0] = 0;
    a[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        a[i] = a[i - 1] + a[i - 2];
    }
    return a[n];
}

int fib3(int n)
{
    int a[n];
    a[0] = 0;
    a[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        a[i % 2] = a[0] + a[1];
    }
    return a[n % 2];
}
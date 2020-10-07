#include <assert.h>
/* 
 * Longest Increasing Substring  
 * 
 * For A,
 * L[i] be the length of the longest increasing subsequence in A[0:i], 
 * including A[i].
 * 
 * Recursive relation:
 * L[i] = 1 + max_{j}{L[j] : A[j] < A[i]}; one plus the maximum previous subsequence
 *  e.g for 
 * 
 * [5 7 4 -3 9 1 10 4 5 8 9 3]
 *  
 */

template <typename T>
int lis(int Ac, int Av[])
{
    assert(Ac > 0);
    int *L = new int[Ac];
    L[0] = 1;
    int maxlen = 1;

    for (int i = 1; i < Ac; i++)
    {
        L[i] = 0; // Initialize all to zero.
        for (int j = i - 1; j >= 0; j--)
        {
            if (Av[i] > Av[j] && L[j] > L[i])
            {
                // Set L to the longest preceding subsequence which ends in
                // a value great than Av[i].
                L[i] = L[j];
            }
        }
        L[i] += 1; // Count the i'th element itself.
        if (L[i] > maxlen)
        {
            maxlen = L[i];
        }
    }

    return maxlen;
}

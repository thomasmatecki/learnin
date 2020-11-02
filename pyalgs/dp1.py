"""
 Dasgupta Papadimitriou Vazirani[DPV]

 Dynamic Programming Practice Problems
  6.1, 6.2, 6.3, 6.4, 6.11
"""

from typing import List

# Maximum contiguous subsequence
def mcs1(xs: List[int]) -> List[int]:
    """
    A contiguous subsequence of a list S is a subsequence made up of
    consecutive elements of S.

    For S:  5, 15, −30, 10, −5, 40, 10,
            THEN 15, −30, 10 is a contiguous subsequence
            BUT 5, 15, 40 is not.

    Give a linear-time algorithm for the following task:
    Input: A list of numbers, a_1 , a_2 , . . . , a_n .
    Output: The contiguous subsequence of maximum sum (a subsequence of length
    zero has sum zero). For the preceding example, the answer would be
    10, −5, 40, 10, with a sum of 55. (Hint: For each j ∈ {1, 2, . . . , n},
    consider contiguous subsequences ending exactly at position j.)
    """
    t = [xs[0:1]]

    def s(i) -> List[int]:
        """
        s(i) is the maximum contiguous subsequence include the ith element of
        xs.
        """
        preceding = t[i - 1]
        if sum(preceding) > 0:
            return preceding + [xs[i]]
        else:
            return xs[i : i + 1]

    max_idx, max_sum = (0, sum(t[0]))
    for i in range(1, len(xs)):
        s_ = s(i)
        sum_ = sum(s_)
        if sum_ > max_sum:
            max_idx = i
            max_sum = sum_

        t.append(s_)

    return t[max_idx]


assert mcs1([5, 15, -30]) == [5, 15]
actual = mcs1([5, 15, -30, 10, -5, 40, 10])
assert actual == [10, -5, 40, 10], f"{actual} not right"


def mcs2(xs: List[int]) -> List[int]:
    """
    Keep track of a table where the i'th entry is a 2-tuple
        [0]: the starting index of the maximum contiguous substring containing
             xs[i]
        [1]: The sum of the maximum contiguous substring containing xs[i]
    """
    t = [(0, xs[0])]
    max_idx = 0

    for i, x in enumerate(xs[1:], 1):
        preceding_start, preceding_sum = t[i - 1]
        if preceding_sum > 0:
            t_i = (preceding_start, preceding_sum + x)
        else:
            t_i = i, x
        t.append(t_i)

        if t_i[1] > t[max_idx][1]:
            max_idx = i

    i, x = t[max_idx]
    return xs[i : max_idx + 1]


actual = mcs2([5, 15, -30, 10, -5, 40, 10])
assert actual == [10, -5, 40, 10], f"{actual} not right"


def optimal_trip(xs: List[int]) -> List[int]:
    """
    You are going on a long trip. You start on the road at mile post 0.
    Along the way there are n hotels, at mile posts a_1 < a_2 <...< a_n,
    where each a i is measured from the starting point.

    The only places you are allowed to stop are at these hotels, but you
    can choose which of the hotels you stop at.

    You must stop at the final hotel (at distance a n ), which is your
    destination.

    You’d ideally like to travel 200 miles a day, but this may not be possible
    (depending on the spacing of the hotels).

    If you travel x miles during a day, the penalty for that day is (200 − x)^2.

    You want to plan your trip so as to minimize the total penalty—that is, the
    sum, over all travel days, of the daily penalties.

    Give an efficient algorithm that determines the optimal sequence of hotels
    at which to stop.

        [190, 250, 410, 700, 850]

    """
    n = len(xs)
    xs = [0] + xs

    def _penalty(i, j):
        """
        i < j
        """
        diff = xs[j] - xs[i]
        return pow(200 - diff, 2)

    def _total_penalty(idxs: List[int]) -> int:
        penalty = 0
        for idx in idxs[1:]:
            penalty += _penalty(idx - 1, idx)

        return penalty

    def optimal_to(i) -> List[int]:
        optimal_path = [*range(i)]
        min_penalty = _total_penalty(optimal_path)
        for j in range(1, i):
            optimal_to_j = optimal_to(j)
            penalty = _penalty(j, i) + _total_penalty(optimal_to_j)
            if penalty < min_penalty:
                min_penalty = penalty
                optimal_path = optimal_to_j

        return optimal_path + [i]

    optimal_trip_ = optimal_to(n)
    return optimal_trip_[1:-1]


print(optimal_trip([190, 250, 410]))
print(optimal_trip([190, 250, 410, 700, 850]))
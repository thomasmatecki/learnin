from random import randint
from typing import List


def bin_search(input_: List[int], e: int) -> bool:

    print("...")
    if len(input_) == 0:
        return -1
    a = len(input_) // 2
    if e < input_[a]:
        print("L")
        return bin_search(input_[0:a], e)
    elif e > input_[a]:
        print("R")
        return bin_search(input_[a + 1 : len(input_)], e)
    else:
        print("X")
        return a


R = 99999
_input = sorted(randint(0, R) for _ in range(100000))

bin_search(_input, randint(0, R))
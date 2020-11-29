from functools import singledispatchmethod, total_ordering
from typing import List, NamedTuple, Tuple


class Table:
    def __init__(self, rows, cols):
        self.cols = cols
        self.data = [0] * rows * cols

    def idx(self, row, col):
        assert col < self.cols, "column out of range"
        return (row * self.cols) + col

    def _get(self, row, col):
        return self.data[self.idx(row, col)]

    def _set(self, row, col, val):
        self.data[self.idx(row, col)] = val

    def __repr__(self):
        remaining = self.data
        rows = []

        while remaining:
            rows.append(",".join(map(repr, remaining[: self.cols])))
            remaining = remaining[self.cols :]

        r = "\n".join(rows)
        return r


class Parcel(NamedTuple):
    weight: int
    value: int


@total_ordering
class Allocation(NamedTuple):
    parcels: Tuple[Parcel] = tuple()

    @property
    def weight(self):
        return sum(p.weight for p in self.parcels)

    @property
    def value(self):
        return sum(p.value for p in self.parcels)

    def __lt__(self, other: "Allocation"):
        return self.weight < other.weight

    def __repr__(self):
        return f"|{self.weight},{self.value}|"

    @singledispatchmethod
    def __add__(self, other):
        raise NotImplementedError()

    @__add__.register
    def _(self, parcel: Parcel):
        return Allocation(parcels=self.parcels + (parcel,))


class Table:
    def __init__(self, rows, cols):
        self.cols = cols
        self.data = [Allocation()] * rows * cols

    def idx(self, row, col):
        assert col < self.cols, "column out of range"
        return (row * self.cols) + col

    def get(self, row, col):
        return self.data[self.idx(row, col)]

    def set(self, row, col, val):
        self.data[self.idx(row, col)] = val

    def __repr__(self):
        remaining = self.data
        rows = []

        while remaining:
            rows.append(",".join(map(str, remaining[: self.cols])))
            remaining = remaining[self.cols :]

        r = "\n".join(rows)
        return r


def knapsack(parcels: List[Parcel], capacity):
    t = Table(len(parcels) + 1, capacity + 1)
    for i_, parcel in enumerate(parcels):
        i = i_ + 1
        for b in range(1, capacity + 1):
            if parcel.weight <= b:
                allocation_incl_parcel = t.get(i - 1, b - parcel.weight) + parcel
                allocation_excl_parcel = t.get(i - 1, b)
                t.set(i, b, max(allocation_excl_parcel, allocation_incl_parcel))
            else:
                t.set(i, b, t.get(i - 1, b))

    return t.get(len(parcels), capacity), t


solution, table = knapsack(
    [
        Parcel(1, 2),
        Parcel(10, 10),
        Parcel(2, 4),
        Parcel(3, 5),
        Parcel(13, 11),
        Parcel(6, 7),
        Parcel(12, 10),
    ],
    17,
)


print(table)
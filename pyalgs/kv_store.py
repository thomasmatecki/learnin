#!/usr/bin/python3.10

from bisect import bisect
from collections import defaultdict, namedtuple
from operator import attrgetter
from time import time_ns


class KVTStore:
    ValueNode = namedtuple("ValueNode", ["timestamp", "value"])
    _node_timestamp = attrgetter("timestamp")

    def __init__(self) -> None:
        self.store = defaultdict(list)

    def set(self, key, value):
        value_node = self.ValueNode(time_ns(), value)
        self.store[key].append(value_node)
        return value_node.timestamp

    def get(self, key, timestamp=None):
        """
        Get the value set most recently as of `timestamp` for the given `key`.

        If `timestamp` not provided, return the latest.
        """
        timestamped_values = self.store[key]

        if not timestamped_values:
            return

        if timestamp is None:
            return timestamped_values[-1].value

        next_timestamp_idx = bisect(
            timestamped_values, timestamp, key=self._node_timestamp
        )

        if next_timestamp_idx == 0:
            return

        return timestamped_values[next_timestamp_idx - 1].value

    def get_proximal(self, key, timestamp):
        """
        Get the value set closest in time (before or after) to `timestamp` for the 
        given `key`.

        If `timestamp` not provided, return the latest.
        """
        timestamped_values = self.store[key]

        if not timestamped_values:
            return

        next_timestamp_idx = bisect( # Python 3.10 Req'd
            timestamped_values, timestamp, key=self._node_timestamp
        )

        if next_timestamp_idx == 0:
            closest_timestamp_idx = 0
        elif next_timestamp_idx == len(timestamped_values):
            closest_timestamp_idx = next_timestamp_idx - 1
        else:
            diff_after = timestamped_values[next_timestamp_idx].timestamp - timestamp

            diff_before = (
                timestamp - timestamped_values[next_timestamp_idx - 1].timestamp
            )

            closest_timestamp_idx = (
                next_timestamp_idx
                if diff_after < diff_before
                else next_timestamp_idx - 1
            )

        return timestamped_values[closest_timestamp_idx].value


if __name__ == "__main__":
    kvt_store = KVTStore()
    assert kvt_store.get("foo") == None

    t_bar = kvt_store.set("foo", "bar")
    assert kvt_store.get("foo") == "bar"
    assert kvt_store.get("goo") == None

    t_baz = kvt_store.set("foo", "baz")
    assert kvt_store.get("foo") == "baz"
    assert kvt_store.get("foo", timestamp=t_bar) == "bar"

    t_bat = kvt_store.set("foo", "bat")
    assert kvt_store.get("foo") == "bat"
    assert kvt_store.get("foo", timestamp=t_bar) == "bar"
    assert kvt_store.get("foo", timestamp=t_baz) == "baz"
    assert kvt_store.get("foo", timestamp=t_bat) == "bat"

    # Get value "as-of" `timestamp`.
    assert kvt_store.get("foo", timestamp=t_bar - 1) == None
    assert kvt_store.get("foo", timestamp=t_bar + 1) == "bar"
    assert kvt_store.get("foo", timestamp=t_baz - 1) == "bar"
    assert kvt_store.get("foo", timestamp=t_baz + 1) == "baz"
    assert kvt_store.get("foo", timestamp=t_bat - 1) == "baz"
    assert kvt_store.get("foo", timestamp=t_bat + 1) == "bat"

    # Get the value "closest" to the timestamp.
    assert kvt_store.get_proximal("foo", timestamp=t_bar - 1) == "bar"
    assert kvt_store.get_proximal("foo", timestamp=t_bar + 1) == "bar"
    assert kvt_store.get_proximal("foo", timestamp=t_baz - 1) == "baz"
    assert kvt_store.get_proximal("foo", timestamp=t_baz + 1) == "baz"
    assert kvt_store.get_proximal("foo", timestamp=t_bat - 1) == "bat"
    assert kvt_store.get_proximal("foo", timestamp=t_bat + 1) == "bat"

    # Closest value, index bounds.
    t_bal = kvt_store.set("goo", "bal")
    assert kvt_store.get_proximal("goo", timestamp=t_bal - 1) == "bal"
    assert kvt_store.get_proximal("goo", timestamp=t_bal) == "bal"
    assert kvt_store.get_proximal("goo", timestamp=t_bal + 1) == "bal"

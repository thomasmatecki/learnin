#! /usr/bin/env python
import random
import time

import redis

r = redis.Redis(host='localhost', port=6379, db=0)
DRIP = 6
THRESHOLD = 10

class Escape:
    ALLOW = '\033[92m'
    BLOCK = '\033[91m'
    LOG = '\033[96m'

class LeakyBucket:

    def request(self, key):
        stamp = time.time()

        with r.pipeline() as pipe:
            pipe.get(f"{key}--last")
            pipe.get(f"{key}--count")
            last_stamp, last_count = map(
                lambda v: float(v or 0),
                pipe.execute(), 
            )

        total_drip = (stamp - last_stamp) * DRIP
        count = max(0.0, last_count - total_drip)

        if count < THRESHOLD:
            count += 1
            with r.pipeline() as pipe:
                pipe.set(f"{key}--last", stamp)
                pipe.set(f"{key}--count", count)
                last, key = pipe.execute()
            color = Escape.ALLOW 
        else:
            color = Escape.BLOCK
        print(f"{color}[block] {Escape.LOG} {stamp:.5f}: {count:.5f} = [{last_count:.5f}@{last_stamp:.5f} - {total_drip:.5f}]")

def main():
    leaky_bucket = LeakyBucket()
    r.flushall()
    while True:
        pause = max(0.0, random.normalvariate(.125, .00125))
        time.sleep(pause)
        leaky_bucket.request("foo")

if __name__ == "__main__":
    main()

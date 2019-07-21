#! /usr/bin/python

from random import randint

INTERVAL_MIN = 0
INTERVAL_MAX = 999999999
f_out = open("../data/intervals1.dat", 'w')

for _ in range(200):
   f_out.write("%d %d\n" % tuple(sorted((
       randint(INTERVAL_MIN, INTERVAL_MAX),
       randint(INTERVAL_MIN, INTERVAL_MAX),
   ))))

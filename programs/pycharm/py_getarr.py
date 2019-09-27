import random

arr = [x for x in xrange(5)]
print arr, " -- a list of consecutive numbers"
arr = [0] * 5
print arr, " -- a list of 0s"
arr = [random.randint(0, 9) for i in xrange(5)]
print arr, " -- a list of random numbers"
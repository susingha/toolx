"""
Given an array of integers and a number k, where 1 <= k <= length of the array,
compute the maximum values of each subarray of length k.

For example, given array = [10, 5, 2, 7, 8, 7] and k = 3,
we should get: [10, 7, 8, 8], since:

10 = max(10, 5, 2)
7 = max(5, 2, 7)
8 = max(2, 7, 8)
8 = max(7, 8, 7)
Do this in O(n) time and O(k) space.
You can modify the input array in-place and
you do not need to store the results.
You can simply print them out as you compute them.
"""
from collections import deque

array = [10, 5, 2, 7, 8, 7]
k = 4

dq = []

for i in range(k):
    while dq and array[i] >= array[dq[-1]]:
        dq.pop(-1)
    dq.append(i)



for i in range(k, len(array)):
    print array[dq[0]]

    if dq[0] <= (i - k):
        dq.pop(0)

    while dq and array[i] >= array[dq[-1]]:
        dq.pop(-1)
    dq.append(i)

print array[dq[0]]

"""
This it not exactly O(n) in the normal sense.
But because of the way the dequeue is used.
One worst case iteration of the dq i.e. k comparisons will ensure that
the next iteration of dq would be 1 comparison - O(1)
the next iteration of dq would be 1 comparison - O(1)
the next iteration of dq would be 1 comparison - O(1)
the next iteration of dq would be 1 comparison - O(1)

Only then we can possibly get another worst case iteration of the dq i.e. k cmoparisons

Hence we will have O(k) iteration of the dq every k iterations of the given array
"""
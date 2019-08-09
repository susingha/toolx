"""
There are a bunch of meeting slots.
They may or may not overlap
find the minimum number of meetings to cancel
to disconnect all meetings

           2                4
     ------------   ----------------
---------    ---------------     ------------
    1              3                  5

instead of finding meetings to remove,
find meetings to keep such that those meetings do not overlap

fn(1) : lets ask 1 how many meetings start after you have finished.
1 will find all meetings after it has ended
those are 3, 4, 5
1 will ask 3 hey how many meetings begins after you have ended.
1 will ask 4 hey how many meetings begins after you have ended.
1 will ask 5 hey how many meetings begins after you have ended.

take the max of those numbers,
add 1
return n

then total meetings - n is the answer

O(n^2)
there is better algo at nlogn
"""

meetings = [(7, 9), (2, 4), (5, 8)]
meetings = [(1, 3), (2, 5), (4, 7), (6, 9), (8, 10)]
meetings.sort(key = lambda x: x[1])
print meetings
lenm = len(meetings)

cache = [1] * lenm
cache[-1] = 1

for i in range(lenm)[::-1]:
    m1 = meetings[i]
    b1 = m1[0]
    e1 = m1[1]

    minc = 0
    for j in range(i+1, lenm):
        m2 = meetings[j]
        b2 = m2[0]
        e2 = m2[1]
        if e1 <= b2:
            minc = max(minc, cache[j])
    cache[i] = minc + 1

print cache
print max(cache)
print lenm - max(cache)

###########################################################

print
intervals = meetings
current_end = -999999
overlapping = 0

for start, end in sorted(intervals, key=lambda i: i[1]):
    print start, end
    if start >= current_end:
        current_end = end
    else:
        overlapping += 1
print overlapping

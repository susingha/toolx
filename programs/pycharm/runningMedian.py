
"""
Compute the running median of a sequence of numbers.
That is, given a stream of numbers,
print out the median of the list so far on each new element.

Recall that the median of an even-numbered list
is the average of the two middle numbers.

For example, given the sequence [2, 1, 5, 7, 2, 0, 5],
your algorithm should print out:

2
1.5
2
3.5
2
2
2

O(nlogn) solution with heap
"""



def insert(heap, num):
    heap.append(num)

def pullminheap(heap):
    m = min(heap)
    heap.remove(m)
    return m

def pullmaxheap(heap):
    m = max(heap)
    heap.remove(m)
    return m

def readminheap(heap):
    return min(heap)
def readmaxheap(heap):
    return max(heap)

arr = [2, 1, 5, 7, 2, 0, 5]
lenarr = len(arr)
maxheap = [] # stores lesser ints to the left
maxcount = 0
minheap = [] # stores larger ints to the right
mincount = 0


median = arr[0]
print median

for i in xrange(1, lenarr):

    if i%2: # i is odd
        insert(minheap, max(median, arr[i]))
        insert(maxheap, min(median, arr[i]))
        l = readmaxheap(maxheap)
        r = readminheap(minheap)

        median = (l+r)/2.0
    else:
        if arr[i] > median:
            insert(minheap, arr[i])
            median = pullminheap(minheap)
        else:
            insert(maxheap, arr[i])
            median = pullmaxheap(maxheap)

    print median







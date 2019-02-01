# Implement heapsort

arr = [30, 91, 15, 72, 61, 41, 10, 37, 98, 41]
lenarr = len(arr)

def leftchild(i):
    return 2*i + 1

def rightchild(i):
    return 2*i + 2

def maxheapify(i, lent):
    max = i
    lc = leftchild(i)
    rc = rightchild(i)

    if lc < lent and arr[lc] > arr[i]:
        max = lc
    if rc < lent and arr[rc] > arr[max]:
        max = rc

    if max != i:
        arr[i], arr[max] = arr[max], arr[i]
        maxheapify(max, lent)


for i in xrange(lenarr):
    maxheapify(lenarr-1-i, lenarr)

for i in xrange(lenarr):
    maxheapify(0, lenarr-i)
    arr[0], arr[lenarr-1-i] = arr[lenarr-1-i], arr[0]

print arr

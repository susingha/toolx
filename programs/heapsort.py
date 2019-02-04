# Implement heapsort

arr = [30, 91, 15, 72, 61, 41, 10, 37, 98, 41]
lenarr = len(arr)

def sorted(arr):
    prev = None
    for i in arr:
        if prev and prev > i:
            print "Not Sorted"
            return False
        prev = i
    print "OK"
    return True

def leftchild(i):
    return 2*i + 1

def rightchild(i):
    return 2*i + 2

def maxheapify_itr(i, lent):
    max = -1

    done = False
    while not done:

        max = i
        lc = leftchild(i)
        rc = rightchild(i)
        if lc < lent and arr[lc] > arr[i]:
            max = lc
        if rc < lent and arr[rc] > arr[max]:
            max = rc

        if i == max:
            done = True
        else:
            arr[i], arr[max] = arr[max], arr[i]
            i = max


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
    maxheapify_itr(lenarr-1-i, lenarr)

for i in xrange(lenarr):
    maxheapify_itr(0, lenarr-i)
    arr[0], arr[lenarr-1-i] = arr[lenarr-1-i], arr[0]

print arr
sorted(arr)

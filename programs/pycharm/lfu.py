"""
notes:
the heap stores top 10 elems
given character, use hash table to find the node in the heap. it may be NULL

heap has 9 elems or less
    insert blindly

heap has 10 elems and count of this node is more than min elem of the heap
    buildheap
    extract: (reset ptr to NULL)
    insert: (set ptr to node)
    heapify:
"""

alpha = "abcdefghijklmnopqrstuvwxyz"
ar = alpha + \
     alpha[:12] + \
     alpha[:11] + \
     alpha[:10] + \
     alpha[:9] + \
     alpha[:8] + \
     alpha[:7] + \
     alpha[:6]
ar = "jskoeurhfdbsbsvxvcgfhgjhkhhldkdwiqiuwytwfsfcvvvfgfhjfjdwuwu"




hasht = {}
class hashnode:
    def __init__(self, al):
        self.al = al
        self.c = 1
        self.ptr = None

class minheap10:
    def __init__(self):
        self.heaparr = []
        self.lenarr = 0
    def nodeval(self, n):
        return n.c
    def buildheap(self):
        self.heaparr.sort(key = self.nodeval)
    def heapify(self, n):
        self.heaparr.sort(key = self.nodeval)

    def fixheap(self, n):
        self.heapify(n)
    def insert(self, n):
        if self.lenarr < 10:
            self.heaparr.append(n)
            self.lenarr += 1
            if self.lenarr == 10:
                self.buildheap()   # nlogn
            return None, n

        if self.lenarr == 10:
            if n.c > self.heaparr[0].c:
                evicted = self.heaparr[0]
                inserted = self.heaparr[0] = n
                self.heapify(self.heaparr[0])  # logn
                return evicted, inserted
            return None, None

    def getNodes(self):
        return self.heaparr[:]

mh10 = minheap10()

def runsequence(arr):
    for al in arr:
        process(al)

def process(al):

    # update hashtable
    if al in hasht:
        hasht[al].c += 1
    else:
        hasht[al] = hashnode(al)

    # insert into minheap10
    n = hasht[al]
    if n.ptr == None:
        evicted, inserted = mh10.insert(n)
        if evicted:
            evicted.ptr = None
        if inserted:
            inserted.ptr = inserted
    else:
        mh10.fixheap(n)

def process2(al):
    """
    hash table update
        not in heap. need to insert ?
            heap has space
                insert -> set pointer
            heap does not have space
                more than min
                evict -> reset pointer
                insert -> set pointer
        in heap
            fix the heap
    """
    pass

runsequence(ar)
print "top 10:"
for x in mh10.getNodes():
    print x.al, x.c, "times"
print
print "all:"
checker = []
for k in hasht:
    checker.append(hasht[k])
def nodecounter(n):
    return n.c
checker.sort(reverse=True, key = nodecounter)
for x in checker[:10]:
    print x.al, x.c, "times"
print
for x in checker[10:]:
    print x.al, x.c, "times"


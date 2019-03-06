

"""
LRU Cache

set:
1. create a new node
2. get the pointer

3. if size > n
    we have to evict
    evict end node
4. insert node at head of the list
5. update hash table for current key as key and value as node

get:
1. goto hashtable lookup key. get the node
2. read value from the node
3. move node to head of the list


evict:
1. take the key from the node
2. lookup hashtable for key. remove the key from the hashtable
3. free the node
"""

class node:
    def __init__(self, key, val):
        self.key = key
        self.val = val
        self.nxt = None
        self.prv = None


class lru:
    def __init__(self, max):
        self.max = max
        self.cn = 0
        self.head = None
        self.last = None

        self.hasht = {}

    def show(self):
        this = self.head
        while this:
            print this.key, this.val
            this = this.nxt

    def evict(self, nod):
        if nod.prv:
            nod.prv.nxt = nod.nxt
        else:
            self.head = nod.nxt

        if nod.nxt:
            nod.nxt.prv = nod.prv
        else:
            self.last = nod.prv
        nod.nxt = None
        nod.prv = None

        self.cn -= 1

    def insert(self, nod):
        nod.nxt = self.head
        nod.prv = None
        if self.head:
            self.head.prv = nod
        self.head = nod
        if self.last == None:
            self.last = nod

        self.cn += 1

    def set(self, key, val):

        new = node(key, val)

        if self.cn >= self.max:
            self.hasht.pop(self.last.key)
            self.evict(self.last)

        self.insert(new)
        self.hasht[key] = new


    def get(self, key):

        if key not in self.hasht:
            return None

        nod = self.hasht[key]

        self.evict(nod)
        self.insert(nod)

        return nod.val


cache = lru(5)
cache.set(1, "food")
cache.set(2, "phone")
cache.set(3, "bed")
cache.set(4, "chair")
cache.set(5, "coffee")
print cache.get(5)
print cache.get(4)
print cache.get(3)
print cache.get(2)
print cache.get(1)
cache.set(6, "fridge")
print cache.get(5)
cache.show()



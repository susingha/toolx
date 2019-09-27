import random

class node:
    def __init__(self, val):
        self.val = val
        self.random = random.randint(20, 50)
        self.ptr = None


arr = []
arr.append(node(0))
arr.append(node(1))
arr.append(node(2))
arr.append(node(3))
arr.append(node(4))

# check the list
print "before modification"
for n in arr:
    print n.val, n.random, n.ptr, n

# add them to a hashtable
hasht = {}
for n in arr:
    hasht[n.val] = n

# change a value on a node in the hashtable
n = hasht[2]
n.random = 100
# change a value on a node in the hashtable directly
hasht[1].random = 500

print
print "after modification in hashtable 1"
# check the list again
for n in arr:
    print n.val, n.random, n.ptr, n




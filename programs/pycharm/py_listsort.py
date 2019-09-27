# demo of python list sort using lambda function (key)

import random

class node:
    def __init__(self, val):
        self.val = val
        self.r = random.randint(10, 40)
        self.ptr = None

arr = []
arr.append(node(2))
arr.append(node(4))
arr.append(node(7))
arr.append(node(1))
arr.append(node(5))
arr.append(node(3))
arr.append(node(8))
arr.append(node(0))

# sort using comparator
def comp1(a, b):
    return a.val - b.val
t = sorted(arr, cmp=comp1)
for x in t:
    print x.val, x.r, x.ptr

print

# sort using lambda value
def nodeval(n):
    return n.val
arr.sort(key = nodeval)

for x in arr:
    print x.val, x.r, x.ptr


st = ["abcd", "aabcd", "aaabcd", "aba", "bca", "abcc"]
def comp2(a, b):
    if a < b:
        return -1
    else:
        return  1
st.sort(cmp = comp2)
print st

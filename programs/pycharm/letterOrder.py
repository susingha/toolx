
"""
You come across a dictionary of sorted words
in a language you've never seen before.
Write a program that returns
the correct order of letters in this language.

For example, given ['xww', 'wxyz', 'wxyw', 'ywx', 'ywz'],
you should return ['x', 'z', 'w', 'y'].

part 1:
compare every two consecutive words
find the first differing letter to get the order.
put the order in hashtable

hashtable
    key: letter
    val: class: v, children set

Part 2:
run dependency graph
call rec(n) on all elems in the hasht.
    if n is visited, return

    get all the children:
    call rec on all the children
    mark as visited.
    prepend to order (sol) array
"""

ar = \
    'xww', \
    'wxyz', \
    'wxyw', \
    'ywx', \
    'ywz'

print ar

hasht = {}
class entry:
    def __init__(self, child):
        self.v = False
        self.children = set()
        self.children.add(child)

def addOrder(a, b):
    global hasht
    if a in hasht:
        hasht[a].children.add(b)
    else:
        hasht[a] = entry(b)

sol = []
def rec(k):
    global sol
    global hasht

    if k not in hasht:
        sol.append(k)
        return

    if hasht[k].v == True:
        return

    for child in hasht[k].children:
        rec(child)
    hasht[k].v = True
    sol.append(k)

def findOrder(arr):
    global hasht

    lenarr = len(arr)

    for i in range(lenarr-1):
        w1 = arr[i]
        w1l = len(w1)
        w2 = arr[i+1]
        w2l = len(w2)

        for li in range(min(w1l, w2l)):
            if w1[li] != w2[li]:
                addOrder(w1[li], w2[li])
                break

    for key in hasht:
        rec(key)

    return sol[::-1]

print findOrder(ar)

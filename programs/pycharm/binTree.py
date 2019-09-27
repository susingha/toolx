
"""
# Count the number of children in lowest level
# The tree may not be a complete and full binary tree

        0
       / \
      /   \
     /     \
    1       2
   / \     / \
  3   4   5   6
 / \
7   8

ans: 2. The node 7 and node 8

Do preorder iteration.
Let each node report their depth to parent.
The parent to notice a different in depth from lc and rc
will know that:
lc is a full binary tree
rc is a full binary tree
but with a difference of depth
depth of the left-full-binary-tree must be 1 + right-full-binary-tree
difference between these numbers is
the number of leaves in the lowest level

"""


n = 9
arr = range(n)
lenarr = len(arr)

def getlc(i):
    lci = (2*i)+1
    if lci < lenarr:
        return lci, arr[lci]
    return -1, None

def getrc(i):
    rci = (2*i)+2
    if rci < lenarr:
        return rci, arr[rci]
    return -1, None

def getDepthLeft(ni):
    d = 0
    if ni >= lenarr:
        return d

    node = arr[ni]

    while (node):
        d += 1
        ni, node = getlc(ni)
    return d

def getDepthRight(ni):
    d = 0
    if ni >= lenarr:
        return d

    node = arr[ni]

    while(node):
        d += 1
        ni, node = getrc(ni)
    return d

stop = False
dl = getDepthLeft(0)
dr = getDepthRight(0)
leafcount = 0

import drawtree

def makeTree(arr, i):
    lcnode = rcnode = None

    lci, lc = getlc(i)
    if lc:
        lcnode = makeTree(arr, lci)

    rci, rc = getrc(i)
    if rc:
        rcnode = makeTree(arr, rci)

    node = drawtree.TreeNode(arr[i])
    if lcnode:
        node.left = lcnode
    if rcnode:
        node.right = rcnode
    return node

head = makeTree(arr, 0)
drawtree.drawtree(head)

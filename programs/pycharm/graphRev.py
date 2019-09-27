
"""
Write an algorithm that computes the
reversal of a directed graph.
For example, if a graph consists of A -> B -> C,
it should become A <- B <- C.

Consider a graph with multiple children,
not a linked list
"""

visited = {}

class node:
    def __init__(self, val, parent):
        self.val = val
        self.children = []
        parent.children.append(self)


def rec(parent, curr):

    if curr in visited:
        curr.children.append(parent)
        return

    # mark visited
    curr.children.append(curr)

    # append into queue from into curr as a new pointer
    # pop from queue as removing old pointer

    childcount = len(curr.children)
    curr.children.append(parent)
    for poptime in xrange(childcount):
        child = curr.children.pop(0)
        rec(curr, child)



"""
Another inferior Approach:
Use DFS / BFS

create use double pointers (back pointers) in graph.

traverse using children array
create a parents array to store the back pointers
add parent to this parents array at every call

traverse using parents array
delete the children array
"""



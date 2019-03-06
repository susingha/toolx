import random

class node:
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None


def insert(head, val):

    new = node(val)

    prev = None
    this = head
    if this == None:
        return new

    while this:
        prev = this

        if val < this.val:
            this = this.left
        else:
            this = this.right


    if val < prev.val:
        prev.left = new
    else:
        prev.right = new

    return head

def display(head):
    if head.left:
        display(head.left)

    print head.val,

    if head.right:
        display(head.right)

def maxn(head, max):
    this = head
    prev2 = None
    prev1 = None

    while this:
        prev2 = prev1
        prev1 = this
        this = this.right

    if max == 1:
        if prev1:
            return prev1.val
        else:
            return None

    if max == 2:
        if prev1:
            if prev1.left:
                return maxn(prev1.left, 1)
            else:
                if prev2:
                    return prev2.val
                else:
                    return None
        else:
            return None

    return None




head = None
lenarr = 10
arr = []

for i in xrange(lenarr):
    arr.append(random.randint(1, 100))
    head = insert(head, arr[i])

display(head)

print
print maxn(head, 2)
arr.sort()
print arr[-2]

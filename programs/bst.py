import random

class node:
    def __init__(self, val):
        self.left = None
        self.right = None
        self.val = val


def treeins(h, val):
    new = node(val)

    this = h
    prev = None
    while this:
        prev = this
        if val < this.val:
            this = this.left
        else:
            this = this.right

    if prev == None:
        return new

    if val < prev.val:
        prev.left = new
    else:
        prev.right = new

    return h

def treedis(node):
    if node.left:
        treedis(node.left)

    print node.val,

    if node.right:
        treedis(node.right)


def treeitr(h):
    stack = []
    done = False

    if h == None:
        return

    this = h
    while True:
        if this and this.left:
            stack.append(this)
            this = this.left
            continue

        print this.val,

        while this.right == None:
            if stack:
                this = stack.pop()
                print this.val,
            else:
                return

        this = this.right


head = None
for i in range(10):
    head = treeins(head, random.randint(1, 100))


print
treedis(head)
print
treeitr(head)


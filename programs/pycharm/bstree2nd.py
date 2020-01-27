import random
import drawtree

'''
class TreeNode(object):
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right
'''

def insert(head, val):

    new = drawtree.TreeNode(val)

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

stack = []
def display_itr(head):
    while True:
        while head:
            stack.append(head)
            head = head.left

        if stack:
            head = stack.pop()
        else:
            head = None

        if head == None:
            return

        print head.val,
        head = head.right

def display_morris(this):
    while this:
        if this.left:
            left = this.left

            temp = left
            while temp.right:
                temp = temp.right
            pred = temp

            pred.right = this
            this.left = None

            this = left
        else:
            print this.val,
            this = this.right



def inord(this, nth, vis):
    ret = None

    if this.right:
        ret = inord(this.right, nth, vis)
        if ret:
            return ret

    vis[0] += 1
    if vis[0] == nth:
        return this

    if this.left:
        ret = inord(this.left, nth, vis)
        if ret:
            return ret

    return ret

def nthlargest(root, nth):
    vis = [0]
    node = inord(root, nth, vis)
    if node:
        return node.val
    return None


head = None
lenarr = 10
arr = []

for i in xrange(lenarr):
    arr.append(random.randint(1, 100))
    head = insert(head, arr[i])



print
print nthlargest(head, 3)
arr.sort()
print arr[-3]

drawtree.drawtree(head)

print
display(head)
print
display_itr(head)
print
display_morris(head)


class node:
    def __init__(self, val):
        self.next = None
        self.val = val



def makell(num):
    head = None
    last = None

    while num:
        new = node(num % 10)
        if last:
            last.next = new
            last = new
        else:
            head = new
            last = new

        num = num/10
    return head



def showll(n):
    while n:
        print n.val,
        n = n.next

def addll(h1, h2):
    head = None
    last = None

    carry = 0
    sum = 0
    while h1 or h2:

        s = carry
        if h1:
            s += h1.val
            h1 = h1.next
        if h2:
            s += h2.val
            h2 = h2.next

        sum = s % 10
        carry = s / 10

        new = node(sum)
        if last:
            last.next = new
            last = new
        else:
            head = new
            last = new

    if carry:
        last.next = node(carry)

    return head


n1 = makell(99990)
print
showll(n1)

n2 = makell(99)
print
showll(n2)

print
ss = addll(n1, n2)
print
showll(ss)







class node:
    def __init__(self, val):
        self.val = val
        self.next = None

def insert(head, val):
    this = head

    if this == None:
        this = node(val)
        return this

    while this.next:
        this = this.next
    this.next = node(val)
    return head

def display(head):
    this = head
    while(this):
        print this.val,
        this = this.next
    print

head = None
head = insert(head, 4)
head = insert(head, 5)
display(head)


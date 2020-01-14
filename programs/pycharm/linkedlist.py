import random

class node:
    def __init__(self, val, next):
        self.val = val;
        self.next = next;


def display(headi):
    this = headi;
    while this:
        print this.val, ",",;
        this = this.next;
    print;


def reverse(headi):
    # prep
    prev = None;
    this = headi;
    # loop
    while this:
        # save
        next = this.next;
        # do
        this.next = prev;
        # prep
        prev = this;
        this = next;

    return prev;





################### main #############################################

random.seed(1000);

#### init section ####
head = None
this = head
MAX = 10;

#### prep section #####
next = None

#### main loop #####
for i in range(MAX):

    ##### do section #####
    x = random.randint(1, 101);
    new = node(x, next);
    head = new;

    ##### prep section ####
    next = new;


display(head);

newhead = reverse(head);
head = newhead;

display(head);










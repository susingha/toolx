"""
Given a list of numbers, create an algorithm that
arranges them in order to form the largest possible integer.
For example, given [10, 7, 76, 415],
you should return 77641510.

sort the full list lexicographically (create a Trie)
if a word ends at a node and node has children.
sort order:
current word at node
child 1
child 2
..

arr = [10, 7, 76, 77, 78, 776, 778, 415]
arr = [10, 7, 76,     78, 776, 778, 415]
arr = [10, 7, 76, 75, 415]
arr = [10, 7, 78, 79, 415]
arr = [10, 7, 76, 79, 415]
arr = [10, 7, 79, 415]          : 797
arr = [10, 123, 1234, 1235, 415]   : 7557172
arr = [10, 719, 729, 739 , 415]

10, 415, 7 76
76 7 415 10

3lists rec(root, stack)
    add self digit to stack # if ends here
    find higher digits child
        rec(child, stack+selfdigit)
        get 3 lists from each child, merge them, get a list for one child (one-child-list)
        merge one-child-list with previous one-child-lists
    call it left list

    find the equal digit child
    rec(child, stack+selfdigit)
    get 3 lists from middle child
        left list (merge it to the left list above)
        middle list
        add stack+selfdigit to the middle list (call it the middle list)
        right list (merge it to the left list below)


    find lower digits childs
        rec(child, stack+selfdigit)
        get 3 lists from each child, merge them, get a list for one child (one-child-list)
        merge one-child-list with previous one-child-lists
    call it the right list

    return leftlist, middlelist, rightlist
"""


arr1 = [10, 7, 76,     78, 776, 778, 415]
arr2 = [10, 7, 76, 77, 78, 776, 778, 415]

def comparat(a, b):
    if str(a) + str(b) > str(b) + str(a):
        return -1
    else:
        return  1

arr1.sort(cmp=comparat)
print arr1

arr2.sort(cmp=comparat)
print arr2

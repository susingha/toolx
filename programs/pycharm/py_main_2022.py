
print
'''
exit(0) # exit with return 0
'''

arr = range(5) # creates an array with ints 0 to 4
arr = range(0, 5, 1) # creates [0, 1, 2, 3, 4]
arr = range(5, 0, -1) # creates [5, 4, 3, 2, 1]
arr = range(4, -1, -1) # creates [4, 3, 2, 1, 0]
arr = [1] * 5 # creates [1, 1, 1, 1, 1]

arr[-1] # prints the last item
arr[-2] # prints the 2nd last item
arr[-5] # prints the 1st item
'''
print arr[-6] # is an error
'''

arr[2], arr[3] = arr[3], arr[2] # will swap the contents at index 2 and 3

arr = [] # empty array with length 0
len(arr) # length of n array. like strlen()

arr = range(5)
arr.pop() # will remove the last element and return the removed element. see doc pop(i)
arr.reverse() # will reverse arr in place. returns None
arr.append(4) # appends 4. return None
arr.sort() # sorts in place. see documentation sort(cmp=None, key=None, reverse=False)

arr = list('immutable') # creates an array ['i', 'm', 'm', 'u', 't', 'a', 'b', 'l', 'e']
str = "".join(arr) # creates an immutable string out of char array


# anti-pattern: Dont modify length of array while traversing it
for i in range(len(arr)):
    print arr[i]
    '''
    arr.pop()
    '''

if 1:
    print "1 is true in if conditions"
if 0:
    print "0 is also true"
else:
    print "0 is false in if conditions"
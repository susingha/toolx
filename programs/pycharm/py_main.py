"""
import heapq
import shuffle

from drawtree import draw_level_order
from drawtree import draw_bst
from drawtree import drawtree



print "do nothing"
li = [55, 30, 10, 5, 2, 20, 15, 25, 40, 35, 70, 60, 80, 75, 95]
shuffle.shuffle(li, 100)
heapq.heapify(li)
print li

draw_bst(li)
# draw_level_order(li)

exit(1)
"""

arr = range(5)
print arr
arr.insert(5, 5)
print arr
arr.insert(100, 6)
print arr


print
for i in range(len(arr)):
    print(arr[i]),

print
for i in reversed(range(len(arr))):
    print(arr[i]),

print
for i in range(len(arr), 0, -1):
    print(arr[i-1]),

print
for i in range(len(arr))[::-1]:
    print(arr[i]),


print
arr = [None]*5
print arr
arr[1] = 9
print arr

print
arr = [1,2,3,4,4,4,5,5]
print arr
arr.remove(min(arr))
print arr
arr.remove(4)
print arr
arr.remove(4)
print arr
arr.remove(4)
print arr

print
xlist = range(0, 10, 2)
print xlist
xlist = range(0, 10, 2)[::-1]
print xlist
xlist = range(0, 10)
print xlist
xlist = range(10, 0, -1)
print xlist

print
for j in xrange(10):
    print j,
    j += 2
print

print
arr = "sup"
print arr
print arr[:]
print arr[:-1]
print arr[0:1]
print arr[::]
print arr[::1]
print arr[::-1]
print arr[::2]
print arr[-2]
print len(arr)
print list(enumerate(arr))
for i, val in enumerate(arr):
    print i, val

### STRING PARSING, USE OF str.split() ###
print
document = "Practice, makes. perfect "
xlist = document.split(' ')
print xlist

xlist = []
dlen = len(document)

def bow(i, dlen):
    if document[i] != ' ' and i == 0:
        return True
    if document[i] != ' ' and document[i-1] == ' ':
        return True
    return False

def eow(i, dlen):
    if i >= dlen:
        return False
    if document[i] != ' ' and i == dlen-1:
        return True
    if document[i] != ' ' and document[i+1] == ' ':
        return True
    return False

for i in range(dlen):
    if bow(i, dlen) == True:
        wb = i

    if eow(i, dlen) == False:
        continue

    we = i
    word = document[wb:we+1]
    xlist.append(word)

print xlist


print
a = [[1, 2, 3, 4], [5, 6], [7, 8, 9]]
print a, len(a), len(a[0])
for row in a:
    for elem in row:
        print elem,
    print
print
quickmatrix = [[0] * 5]
print quickmatrix
print





### DICTIONARY / HASHMAP ###
print
counts = {}
print counts.get('abc')
counts['abc'] = 'this string'
print counts.get('abc')
counts[4] = 'next string'
print counts.get(4)

print counts
for items in counts:
    print items, counts[items]


### ASCII Values ###
diff = ord('c') - ord('b')
print diff
print ord('A')
print chr(65)


print
print 7/5
print 7.0/5
print 7/5.0

print
print min(7,5,8,2)

# convert int to str
print
result = ""
cat1 = "abc"
cat2 = "def"
cat3 = 89
print cat1+cat2
result += cat1
print result
result += cat2
print result
result += cat3.__str__()
print result


# convert str to int
print
c = '5'
n = int(c)
print n, n+1

print
arr = "abcd"
print range(len(arr) - 1, -1, -1)

print
for i in xrange(0, 10):
    print i,
print
for i in xrange(10-1, 0-1, -1):
    print i,

print
s1 = [1,[2],3,4, []]
s2 = [5,6,7]
print s1+s2

print
teststr = "abcd"
print teststr
testlist = list(teststr)
print testlist
print testlist + testlist[1:2]

print
ar1 = [10, 2, 5, 9, 2, 1, 4]
ar2 = ar1[:]
ar2.sort()
print ar2
print ar1

print
n = 16
print bin(n), hex(n),
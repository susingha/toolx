"""

Let's define a "sevenish" number to be
one which is either a power of 7,
or the sum of unique powers of 7.
The first few sevenish numbers are 1, 7, 8, 49, and so on.
Create an algorithm to find the nth sevenish number.

0

1
01

2
02
12
012

3
03
13
013
23
023
123
0123

4
"""

def calculate(ar):
    num7 = 0
    for x in ar:
        num7 += (7**x)
    print num7

def print7nums(n):
    arr = []
    c = 0
    for i in xrange(9999):

        t = [i]
        calculate(t)
        c += 1
        if c == n:
            return

        # deep copy arr into arr2
        arr2 = []
        for x in arr:
            xcopy = x[:]
            arr2.append(xcopy)

        # go over each entry (array) of arr2 and append i
        for j in arr2:
            j.append(i)
            calculate(j)
            c += 1
            if c == n:
                return

        # append the element
        arr.append(t)
        # append the deep copied array
        arr += arr2

print7nums(10)

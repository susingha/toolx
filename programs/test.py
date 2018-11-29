#!//usr/bin/python


# 0, 1, 2, 3, 4, 5
# a  b  c  d  e

def swap(arr, a, b):
    temp = arr[a]
    arr[a] = arr[b]
    arr[b] = temp

def findend(arr, b):

    if (arr[b] == ' '):
	return b

    for i in range(b, len(arr)):
	if (arr[i] == ' '):
	    return i-1
    return i

def mirror(arr, b = -1, e = -1):

    if (e == -1 or e == -1):
	b = 0
	e = len(arr) - 1

    print "mirror ", b, " to ", e
    for i in range(b, 1+(b+e)/2):
        swap(arr, i, e-(i-b))
    print arr

def reverse_words(arr):

    print arr, len(arr)
    mirror(arr)

    begin = 0
    end = 0
    while begin < len(arr):
	end = findend(arr, begin)
	mirror(arr, begin, end)
	begin = end + 1
	if (begin >= len(arr)):
	    break
    print arr, len(arr)




arr = ['a', ' ', ' ', 'd']
arr = ['p', 'e', 'r', 'f', 'e', 'c', 't', ' ', 'm', 'a', 'k', 'e', 's', ' ', 'p', 'r', 'a', 'c', 't', 'i', 'c', 'e']
arr = [' ', ' ']


reverse_words(arr)




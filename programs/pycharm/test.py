import random

arr = []

size = random.randint(0, 5)
for i in range(size):
    arr.append(random.randint(0, 2*size))

arr.sort()
print arr
x = random.randint(0, 2*size)


lenarr = len(arr)
b = 0
e = lenarr - 1
found = False

while b <= e:

    mid = (b+e)/2

    if arr[mid] == x:
        print x, " at index", mid
        found = True
        break

    if x < arr[mid]:
        e = mid-1
    elif x > arr[mid]:
        b = mid+1

if not found:
    print x, "not found"
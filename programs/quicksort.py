import sys
import random


def swapindex(index1, index2, array):
    # print ("swap " + str(index1) + ", " + str(index2))
    temp = array[index1]
    array[index1] = array[index2]
    array[index2] = temp

def printarray(array, total):
    print(array)


def checksorted(array):
    for i in range(len(array) - 1):
        if (array[i] > array[i + 1]):
            print("sup: Array not sorted")
            return
    print("sup: Array sorted")


def sort(start, end, arr):
    if (start == end):
        return

    final = 0
    """
    pivot = start
    """
    pivot = (start + end) / 2
    leftprobe = start
    rightprobe = end

    while (leftprobe < rightprobe):
        while (arr[leftprobe] <= arr[pivot] and leftprobe < end):
            leftprobe += 1
        while (arr[rightprobe] >= arr[pivot] and start < rightprobe):
            rightprobe -= 1

        if (leftprobe < rightprobe):
            swapindex(leftprobe, rightprobe, arr)

    if (pivot < rightprobe):
        final = rightprobe
    elif (pivot > leftprobe):
        final = leftprobe
    else:
        final = pivot

    swapindex(pivot, final, arr)

    if (start < final):
        sort(start, final - 1, arr)
    if (final < end):
        sort(final + 1, end, arr)


total = 100
arr = []
for i in range(0, total):
    num = random.randint(1,10)
    arr.append(num)
    # print(str(i) + " - " + str(num))

printarray(arr, total)
checksorted(arr)
sort(0, len(arr)-1, arr)
printarray(arr, total)
checksorted(arr)

print ("\n")
print ("Python current version:")
print (sys.version)
print (sys.version_info)
print (sys.hexversion)
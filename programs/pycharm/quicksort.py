import random

def checksorted(array):
    for i in range(len(array) - 1):
        if (array[i] > array[i + 1]):
            print("Array is not sorted")
            return
    print("Array is sorted")


def sort(array, b, e):

    # Finding Pivot
    # Take last element as pivot
    # Assume everything is more or equal to the pivot
    # initialize hold at 0 (indicates first element of higher elements)
    # initialize probe at 0
    piv = e
    hold = b
    for probe in range(b, e):
        if array[probe] < array[piv]:
            array[probe], array[hold] = array[hold], array[probe]
            hold += 1

    # placing pivot
    array[piv], array[hold] = array[hold], array[piv]

    # recursion
    if b < hold-1:
        sort(array, b, hold-1)
    if hold+1 < e:
        sort(array, hold+1, e)


total = 10
arr = []
for i in range(total):
    num = random.randint(1,100)
    arr.append(num)

print arr
checksorted(arr)
sort (arr, 0, total-1)
print arr
checksorted(arr)




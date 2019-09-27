import random

ar = [55, 30, 10, 5, 2, 20, 15, 25, 40, 35, 70, 60, 80, 75, 95]
ar = [1, 2, 3, 4, 5]

def shuffle(arr, times = 5):
    lenarr = len(arr)

    s = random.randint(0, lenarr-1)
    carry = arr[s]
    while times:
        times -= 1

        n = random.randint(0, lenarr-1)
        save = arr[n]
        arr[n] = carry
        carry = save

        if n == s:
            break

    if n != s:
        arr[s] = carry

    return arr

print shuffle(ar)
# pick a targer position to place the carry
# save the value at target position
# move the carry to the target position
# saved value is the new carry
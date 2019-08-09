# this is basically 0/n knapsack

ints = [1, 2, 3, 4]
ints.sort()
minx = min(ints)

def split(n, t, stack):
    if t < 0 or n == 0:
        return 0

    if t == 0:
        print stack
        return 1

    a = split(n-1, t, stack)

    stack.append(ints[n-1])
    b = split(n, t - ints[n-1], stack)
    stack.pop()

    return a+b



print split(len(ints), 7, []), " - total"
print split(len(ints), 3, []), " - total"


coins = [1, 5, 10, 25]

def splitmin(n, t):
    if t == 0:
        return 0

    if t < 0 or n == 0:
        return 999999

    # take nth coin
    # new target: t - ints[n-1]
    # new set: all n coins
    a = 1 + splitmin(n, t-coins[n-1])

    # dont take nth coin
    # new target: t
    # new set: n-1 coins
    b = splitmin(n-1, t)

    return min(a, b)

print splitmin(len(coins), 16)
print splitmin(len(coins), 17)
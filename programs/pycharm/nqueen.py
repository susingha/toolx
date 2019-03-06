n = 4
arr = [[0]*n for i in range(n)]
print arr


def safeQueen(row, col):
    global arr
    global n

    for r in range(row-1, -1, -1):
        if arr[r][col] == 1:
            return False
        if col+(row-r) < n and arr[r][col+(row-r)] == 1:
            return False
        if col-(row-r) >= 0 and arr[r][col-(row-r)] == 1:
            return False

    return True


def placeQueen(row, col, queenNo):
    global arr
    global n

    arr[row][col] = 1

    if not safeQueen(row, col):
        arr[row][col] = 0
        return 0

    if queenNo == n: # we have placed the last queen successfully
        print
        for r in range(n):
            print arr[r]
        arr[row][col] = 0
        return 1

    if row == n-1:
        arr[row][col] = 0
        return 0

    count = 0
    for c in range(n):
        count += placeQueen(row+1, c, queenNo+1)
    arr[row][col] = 0
    return count


count = 0
for c in range(n):
    count += placeQueen(0, c, 1)
print
print "count:", count

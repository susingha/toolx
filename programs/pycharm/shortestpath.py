# Shortest path graph traversal using adjacency list

N = 10
arr = [[i, 99, []] for i in range(N)]

def resetarr():
    global arr
    global N
    for i in range(N):
        arr[i][1] = 99

def conn(a, b):
    if b not in arr[a][2]:
        arr[a][2].append(b)
    if a not in arr[b][2]:
        arr[b][2].append(a)

conn(0, 1)
conn(1, 2)
conn(2, 3)
conn(3, 4)
conn(4, 5)
conn(5, 6)
conn(6, 7)
conn(7, 8)
conn(8, 9)
conn(9, 0)
conn(2, 6)
conn(9, 4)

bestsol = None
solution = [None] * N


def traverse(nodeid, depth, destid):
    global bestsol
    global solution

    solution[depth] = nodeid

    if depth < arr[nodeid][1]:
        arr[nodeid][1] = depth

        if nodeid == destid:
            bestsol = solution[:]
        else:
            for adjid in arr[nodeid][2]:
                traverse(adjid, depth+1, destid)

    solution[depth] = None
    return

resetarr()
traverse(0, 0, 6)
print bestsol

resetarr()
traverse(0, 0, 5)
print bestsol
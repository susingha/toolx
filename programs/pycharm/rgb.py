arr = ['G', 'B', 'R', 'R', 'B', 'R', 'G']
lenarr = len(arr)

lr = -1
lg = -1
lb = -1
for i in xrange(lenarr):
    if arr[i] == 'B':
        lb += 1
    if arr[i] == 'G':
        arr[lb+1], arr[lg+1] = arr[lg+1], arr[lb+1]
        lb += 1
        lg += 1
    if arr[i] == 'R':
        arr[lb+1], arr[lg+1] = arr[lg+1], arr[lb+1]
        arr[lg+1], arr[lr+1] = arr[lr+1], arr[lg+1]
        lb += 1
        lg += 1
        lr += 1

print arr




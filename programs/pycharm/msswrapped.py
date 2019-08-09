
arr = [8, 1, -2, 3, 4]

arr = [2, 3, 4, -100, 1000]
arr = [8, -1, 3, 4]
arr = [8, 1, -2, 1, -3, 3, 4]


lenarr = len(arr)

cursum = maxsum = 0
for e in range(lenarr):
    cursum += arr[e]
    cursum = max(0, cursum)

    maxsum = max(maxsum, cursum)
print maxsum

cursum = minsum = 0
for e in range(lenarr):
    cursum += arr[e]
    cursum = min(0, cursum)

    minsum = min(cursum, minsum)
print minsum

print max(sum(arr) - minsum, maxsum)
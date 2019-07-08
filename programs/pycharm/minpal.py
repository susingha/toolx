


arr = "racecarannakayakc"

lenarr = len(arr)

cache1 = []
for r in range(lenarr):
    cache1.append([0] * lenarr)


offset = -1

for limit in reversed(range(lenarr)):
    offset += 1
    for i in range(limit+1):
        r = i
        c = i + offset

        if r == c:
            cache1[r][c] = 1
            continue
        if arr[r] == arr[c]:
            if c == r+1:
                cache1[r][c] = 2
            elif cache1[r + 1][c - 1] != 0:
                cache1[r][c] = cache1[r + 1][c - 1] + 2

print
print arr
print
for i in range(lenarr):
    print cache1[i]

cache2 = []
cache2.append([0] * lenarr)
cache2.append([None] * lenarr)

def lookup(b):
    if b >= lenarr:
        return 0, b

    if cache2[0][b] != 0:
        return cache2[0][b], cache2[1][b]

    r = b
    minno = 9999999
    mincon = -1
    for c, val in enumerate(cache1[r]):
        if val != 0:
            no, con = lookup(c+1)
            if no < minno:
                minno = no
                mincon = c+1

    cache2[0][b] = minno + 1
    cache2[1][b] = mincon
    return cache2[0][b], cache2[1][b]

print lookup(0)
print cache2[0]
print cache2[1]
print

i = 0
while i < lenarr:
    b = i
    n = cache2[1][b]
    print arr[b:n],
    i = n

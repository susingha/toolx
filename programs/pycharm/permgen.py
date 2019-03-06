"""
permutation generator
permutation iterative
"""

lenarr = 0
larr = []
lstr = []

def rotate(l, ri):
    t = l[lenarr-1]
    for j in range(lenarr-1, ri, -1):
        l[j] = l[j-1]
    l[ri] = t

def findNonZeroIndex(l):
    j = lenarr - 1
    while j >= 0 and l[j] == 0:
        j -= 1
    return j

def rotateLen(l, rl):
    l[lenarr - rl] = rl

def getperm(string = ""):
    global lenarr
    global larr
    global lstr

    if string != "":
        lstr = list(string)
        lenarr = len(lstr)
        larr = [0]*lenarr
        rotateLen(larr, lenarr)
        return "Done"

    while (True):
        j = findNonZeroIndex(larr)
        if j >= 0:
            if j == lenarr-1:
                larr[j] -= 1
                return ''.join(lstr)
            else:
                rotate(lstr, j)
                larr[j] -= 1
                rotateLen(larr, lenarr - (j+1))
        else:
            return None

res = getperm("abcd")
while res != None:
    res = getperm()
    print res

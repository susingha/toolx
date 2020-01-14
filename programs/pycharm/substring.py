#!/bin/python

"""
Find the longest nonrepeating substring in a string
"""

maxlen = 0
maxb = 0
maxe = 0

chash = {}
nrepeats = 0
for i in range(ord('a'), ord('z')+1):
    chash[chr(i)] = 0

def hasRepeats(bi, ei, newi):
    global str
    global nrepeats

    if newi == None:
        chash[str[ei]] = 1

    elif newi == 'Begin':
        chash[str[bi-1]] -= 1
        if chash[str[bi-1]] == 1:
            nrepeats -= 1

    elif newi == 'End':
        chash[str[ei]] += 1
        if chash[str[ei]] == 2:
            nrepeats += 1

    if nrepeats > 0:
        return True
    else:
        return False


def updateMax(bi, ei):
    global maxlen
    global maxb
    global maxe
    leni = ei - bi + 1
    if leni > maxlen:
        maxlen = leni
        maxb = bi
        maxe = ei


str = "abcdfghdkolp"
str = "abcdfghdko"
str = "aabaaakal"
strlen = len(str)
b = 0
e = 0
new = None
while e < strlen:
    if hasRepeats(b,e,new) == True:
        b += 1
        new = 'Begin'
    else:
        updateMax(b,e)
        e += 1
        new = 'End'

print str[maxb:maxe+1]
print "length", maxlen

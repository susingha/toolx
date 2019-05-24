
"""
2 // "aaba" - 2
3 // "abac" - 3
3 // "baca" - 3
3 // "acab"
4 // "cabd"
3 // "abda"
4 // "bdab"
4 // "dabc"
3 // "abca"
3 // "bcaa"
2 // "caaa"
"""

s = "aabacabdabcaaa"
lens = len(s)
k = 4

hasht = {}
distch = 0
maxdistch = 0
b = 0
e = 0

for i in range(26):
    hasht[chr(ord('a')+i)] = 0

for i in range(lens):

    hasht[s[i]] += 1
    if hasht[s[i]] == 1:
        distch += 1

    if i >= k:
        hasht[s[i-k]] -= 1
        if hasht[s[i-k]] == 0:
            distch -= 1

    if distch > maxdistch:
        maxdistch = distch
        e = i
        b = i-k+1

print maxdistch
print s[b:e+1]


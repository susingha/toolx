
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


s = "aabacabdabcaaa"
lens = len(s)
k = 4

hash = [0] * 26
distch = 0

for i in xrange(lens):

    hash[ord(s[i]) - ord('a')] += 1
    if hash[ord(s[i]) - ord('a')] == 1:
        distch += 1

    if i == k - 1:
        print s[:k], distch

    if i >= k:
        j = i - k

        hash[ord(s[j]) - ord('a')] -= 1
        if hash[ord(s[j]) - ord('a')] == 0:
            distch -= 1

        print s[j:i], distch



"""


def isnum(c):
    if ord('0') <= ord(c) and ord(c) <= ord('9'):
        return True
    return False

def getnum(c):
    return int(c)


s = "2+2*2*2+2"
lens = len(s)

op1 = 0
operation = '+'
resultadd = 0
resultmul = 1

i = 0
while i < lens:

    if isnum(s[i]):
        op1 = op1 * 10 + getnum(s[i])

    if s[i] == '+' or s[i] == '*':

        if operation == '*' and s[i] == '*':
            resultmul = resultmul * op1

        if operation == '+' and s[i] == '+':
            resultadd = resultadd + op1

        if operation == '+' and s[i] == '*':
            resultmul = op1

        if operation == '*' and s[i] == '+':
            resultmul = resultmul * op1
            resultadd = resultadd + resultmul
            resultmul = 1

        operation = s[i]
        op1 = 0

    i += 1

if operation == '*':
    resultmul = resultmul * op1
    resultadd = resultadd + resultmul
if operation == '+':
    resultadd = resultadd + op1

print resultadd


















wordset = {'quick', 'brown', 'the', 'fox', 'th', 'foxx'}
str = "thequickbrownfox"

wordset = {'bed', 'bath', 'bedbath', 'and', 'beyond'}
str = "bedbathandbeyond"

lenstr = len(str)
sol = []


def getindxlist (s):
    ilist = []
    for i in range(s, lenstr):
        if str[s:i+1] in wordset:
            ilist.append(i)
    return ilist


def lookup (s):
    if s == lenstr:
        return True

    ret = False
    ilist = getindxlist(s)
    for i in ilist:
        ret = lookup(i+1)
        if ret == True:
            sol.append(i)
            break

    return ret


lookup (0)
prev = 0
for i in sol[::-1]:
    print str[prev:i+1],
    prev = i+1
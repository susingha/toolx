text = "abcdeeefghefgheeefgh"
patt = "abcd*efgh"
patt = "*ab??*efg?*"

lentext = len(text)
lenpatt = len(patt)


def charmatch(pi, ti):
    global text, patt, lenpatt, lentext

    if pi >= lenpatt:
        return False
    if ti >= lentext:
        return False

    if text[ti] == patt[pi]:
        return True
    if patt[pi] == "?":
        return True

    return False

def findch(ch, ti):
    while ti < lentext and text[ti] != ch:
        ti += 1

    return ti

def lookup(pbegin, tbegin):
    global text, patt, lenpatt, lentext
    print
    print text[tbegin:]
    print patt[pbegin:]

    pi = pbegin-1
    ti = tbegin-1
    match = True

    while match:
        pi += 1
        ti += 1
        match = charmatch(pi, ti)

    if pi == lenpatt and ti == lentext:
        print "True"
        return True

    if pi >= lenpatt:
        print "False"
        return False

    if patt[pi] == '*':
        if pi+1 == lenpatt:
            print "True"
            return True
        else:
            while ti < lentext:
                ti = findch(patt[pi + 1], ti)
                if lookup(pi+1, ti):
                    print "True"
                    return True
                ti += 1
            print "False"
            return False
    else:
        print "False"
        return False


print lookup(0, 0)


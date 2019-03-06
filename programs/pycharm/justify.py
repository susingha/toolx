




def addwords(currwordcount, i, currlen):
    global result
    global words

    justifiedstring = ""

    spaces = 16 - currlen - 1
    places = currwordcount + 1
    mod = spaces % places
    div = spaces / places
    n = 0

    print "currlen:", currlen
    print "spaces:", spaces
    print "places:", places
    print "mod:", mod
    print "div:", div

    for j in xrange(i - currwordcount, i):

        if mod:
            spacecount = div + 1
        else:
            spacecount = div
        print spacecount
        justifiedstring += ' '*spacecount
        if mod:
            mod -= 1
        n = 1

        justifiedstring += ' '*n + words[j]

    if mod:
        spacecount = div + 1
    else:
        spacecount = div
    print spacecount

    justifiedstring += ' ' * spacecount

    result.append(justifiedstring)
    print



k = 16
words = ["the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"]
lenwords = len(words)


currlen = 0
currwordcount = 0
result = []
i = 0
while i < lenwords:
    wlen = len(words[i])

    if currwordcount > 0:
        wlen += 1

    if currlen + wlen <= 16:
        currlen += wlen
        currwordcount += 1
        i += 1
    else:
        addwords(currwordcount, i, currlen)
        currlen = 0
        currwordcount = 0

addwords(currwordcount, i, currlen)

for i in result:
    print i, len(i)
print result


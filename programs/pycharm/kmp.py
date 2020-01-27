# Implement KMP


texts = [None]*10
texts[0] = "aaaaapaa"
texts[1] = "aaaaakaa"
texts[2] = "aaaaakba"
texts[3] = "aaaaakbaa"
texts[4] = "aaaaakbaaaakba"
texts[5] = "aaaaaka"

text = texts[4]
pat = "aaaakba"

lentext = len(text)
lenpat = len(pat)

# phase 1

csp = [0] * lenpat
hold = 0

for probe in xrange(1,lenpat):

    if pat[probe] == pat[hold]:
        csp[probe] = csp[probe-1] + 1
        hold += 1
    elif pat[0] == pat[probe]:
        csp[probe] = 1
        hold += 1
    else:
        hold = 0
        csp[probe] = 0


# phase 2

ti = 0
pi = 0
while ti < lentext:

    if pi < lenpat and text[ti] == pat[pi]:
        if pi == lenpat - 1:
            print text[:ti+1-lenpat], text[ti+1-lenpat:ti+1], text[ti+1:]
        ti += 1
        pi += 1
        continue
    elif pi > 0:
        pi = csp[pi-1]
        continue
    else:
        ti += 1







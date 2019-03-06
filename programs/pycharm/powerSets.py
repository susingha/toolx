st = [1, 2, 3, 4]

def powersets(s):
    if len(s) == 0:
        return [[]]

    p1 = powersets(s[1:])
    p2 = []
    for sets in p1:
        p2.append(sets+[s[0]])
    return p1+p2

result = powersets(st)
print result
print
for sets in result:
    print sets



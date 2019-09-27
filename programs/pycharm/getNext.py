# This may be useful as a url generator in orl shorteners
# Given a short url, we wanna generate the next one
# start with aaaaaaa

pat = "aaaaa"
patlist = list(pat)
lenlist = len(patlist)

counter = 0
while True:
    counter += 1
    print "".join(patlist), counter

    carry = True
    idx = lenlist-1
    while carry == True and idx >= 0:
        if patlist[idx] == 'z':
            patlist[idx] = 'a'
            carry = True
            idx -= 1
        else:
            patlist[idx] = chr(ord(patlist[idx]) + 1)
            carry = False







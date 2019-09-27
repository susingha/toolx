arr = "/usr/../bin/../bin/./scripts/../"
arr = "/usr/bin/../bin/./scripts/../"

input = arr.split('/')
input.pop(0)
if arr[-1] == '/':
    input.pop()
print input


output = []
lenout = 0
wptr = 0
for foldername in input:
    if foldername == '.':
        continue
    if foldername == '..':
        wptr -= 1 # check -ve
        if wptr == -1:
            print "invalid path"
            break
        continue

    # write foldername at offset wptr in output
    if wptr >= lenout:
        output.append(foldername)
        lenout += 1
        wptr += 1
    else:
        output[wptr] = foldername
        wptr += 1

print output[:wptr]
print "/" + "/".join(output[:wptr]) + "/"
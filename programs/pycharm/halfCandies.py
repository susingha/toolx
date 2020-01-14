arr = [80, 81, 82, 100000, 80, 80, 80, 232342]
arr = [80, 80, 80, 100000, 80, 80, 80, 232342]


"""
arr list of types of chocolates
80 is a type of chocolate
100000 is another type of chocolate
There are even number of chocolates
in arr you have 8 chocolates. And there are 3 types. 80, 100000, 232342
You have to give away half the chocolates.
You want to retain as many varieties as you can for yourself.
Print the max number of varieties you can retain.

[80, 80, 80, 100000, 80, 80, 80, 232342] ans: 3 out of 3
[80, 81, 82, 100000, 80, 80, 80, 232342] ans: 4 out of 5

"""

counts = {}
given = 0
arrlen = len(arr)

for i in range(len(arr)):
    if arr[i] in counts:
        given += 1
    else:
        counts[arr[i]] = 1

if given >= arrlen/2:
    print len(counts)
    quit()

diff = arrlen/2 - given
print (len(counts) - diff)





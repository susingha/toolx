import random




def find(array, b, e):
    size = e-b+1;
    offset = e-b;
    mid = (b+e)/2;

    if array[b] + offset == array[b + offset]:
        return -1;

    if size == 2:
        return array[b] + 1;

    offset = mid-b;
    if array[b + offset] == array[b] + offset:
        # look right
        return find (array, mid, e);
    else:
        # look left
        return find(array, b, mid);



############ main #########

begin = random.randint(5, 10);
size  = random.randint(5, 10);

arra = [];

for i in range(begin, begin+size):
    arra.append(i);

size = len(arra)

if random.randint(0, 10) % 2:
    missing = random.randint(arra[1], arra[size - 2])
    arra.remove(missing)
    print "missing", missing


print arra
size = len(arra)

print "find the missing number in the series of consecutive numbers"
print "missing", find(arra, 0, size-1);



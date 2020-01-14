import random

def merge(array1, b1, len1, array2, b2, len2, final, f1):

    i = b1;
    j = b2;
    k = f1;

    while i < b1+len1 and j < b2+len2:

        if array1[i] <= array2[j]:
            final[k] = array1[i];
            i += 1;
            k += 1;
        else:
            final[k] = array2[j];
            j += 1;
            k += 1;

    while i < b1+len1:
        final[k] = array1[i];
        i += 1;
        k += 1;

    while j < b2+len2:
        final[k] = array2[j];
        j += 1;
        k += 1;


def msort(array1, b, e, array2):
    if b >= e:
        return;

    m = (b + e)/2;
    msort(array2, b,   m, array1);
    msort(array2, m+1, e, array1);

    merge(array1, b,   m-b+1,
          array1, m+1, e-(m+1)+1, array2, b);



######### main ########

print "Merge two sorted arrays" # merge two sorted arrays

arr1 = []; MAX1 = random.randint(0, 5);
arr2 = []; MAX2 = random.randint(0, 5);
final = [];

for i in range(MAX1):
    x = random.randint(1, 100);
    arr1.append(x);
    final.append(0);

for i in range(MAX2):
    y = random.randint(1, 100);
    arr2.append(y);
    final.append(0);


arr1 = sorted(arr1)
arr2 = sorted(arr2)

print arr1
print arr2

merge(arr1, 0, len(arr1), arr2, 0, len(arr2), final, 0);

print "Merging the above two sorted arrays"
print final, "/", len(final);
print "Checking sortedness"
print sorted(final);


print
print "Mergesort an array" # Mergesort an aray
array1 = []
array2 = []
MAX = random.randint(0, 10)

for i in range(MAX):
    num = random.randint(1, 100);
    array1.append(num);
    array2.append(num);

print array1
print "Sorting the above array"
msort(array1, 0, MAX-1, array2)
print array2
print "Checking sortedness"
print sorted(array2)





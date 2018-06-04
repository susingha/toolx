import random;

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




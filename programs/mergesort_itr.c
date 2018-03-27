#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ARR_SIZE (25)
#define ZERO (0)
#define ONE (1)
#define TWO (2)

// Prints the array and checks for sprtedness
int printarray(int * array) {
    int i;
    int sorted = 1;
    int prev = -1;
    for(i = 0; i < ARR_SIZE; ++i) {
	printf("%d, ", array[i]);
	if (array[i] < prev) {
	    sorted = 0;
	}
	prev = array[i];
    }
    if (sorted) {
	printf("\nok\n");
	return 0;
    } else {
	printf("\nNOT sorted\n");
	return -1;
    }

    return -1;
}


// Merges two sorted arrays into a single sorted array
void merge(int arr1[], int size1, int arr2[], int size2, int res[]) {
    int i = 0, j = 0, k = 0;

#ifdef DEBUG
    printf("sup: merge ");
    if (arr1)
        printf("%d to %d and ", arr1[0], arr1[size1-1]);
    else
        printf("NULL and");
    
    if (arr2)
        printf("%d to %d\n", arr2[0], arr2[size2-1]);
    else
        printf("NULL\n");
#endif

    while (i < size1 && j < size2 && arr1 && arr2) {
	if (arr1[i] <= arr2[j]) {
	    res[k] = arr1[i];
	    i++;
	    k++;
	} else {
	    res[k] = arr2[j];
	    j++;
	    k++;
	}

    }
    while (i < size1 && arr1) {
	res[k] = arr1[i];
	i++;
	k++;
    }
    while (j < size2 && arr2) {
	res[k] = arr2[j];
	j++;
	k++;
    }
}

int switchcopy(int i) {
    return 1 - i;
}

int * trimarray(int arr[], int begin, int * size_, int fullsize) {
    int size = *size_;
    int end  = begin + size - 1;

    if (begin >= fullsize)
        return NULL;

    if (end >= fullsize)
        end = fullsize - 1;

    size = end - begin + 1;
    *size_ = size;
    return &arr[begin];
}

// Mergesort in O(n) space using 2 arrays.
// returns and integer to identify which array to find the sorted array in
int mergesort_itr (int A[TWO][ARR_SIZE], int fullsize) {

    int samplesize = 1, arraysize;
    int copyfrom = ZERO;
    int begin, begin1, size1, begin2, size2;
    int mdst, msrc = 1;
    int *arr1, *arr2, *res;

    while (samplesize <= fullsize) {

        msrc = switchcopy(msrc);
        mdst = 1 - msrc;

        samplesize = samplesize * 2;
        begin = 0;
#ifdef DEBUG
        printf("sup: samplesize = %d\n", samplesize);
#endif
        while (begin < fullsize) {

            begin1 = begin;
            size1 = samplesize / 2;

            begin2 = begin1 + size1;
            size2 = samplesize / 2;

            arr1 = trimarray(A[msrc], begin1, &size1, fullsize);
            arr2 = trimarray(A[msrc], begin2, &size2, fullsize);
            res  = &A[mdst][begin1];

            merge(arr1, size1, arr2, size2, res);

            begin = begin + samplesize;
        }
#ifdef DEBUG
        printarray(A[mdst]);
#endif
    }
    return mdst;
}


#if 0
// Test function for merging two sorted arrays
int main() {
    int i;
    int arr[10] = {0,1,2,3,4,5,6,7,8,9};
    int res[10];
    merge_(&arr[0], 5, &arr[5], 5, res);
    for (i = 0; i < 10; i++) {
            printf("%d, ", res[i]);
    }
    printf("\n");
}

#else

// Test function to sort an array using mergesort
int main() {
    int array[TWO][ARR_SIZE]; // indices: 0 to 49
    int i;

    srand(time(NULL));

    for(i = 0; i < ARR_SIZE; ++i) {
	array[ZERO][i] =
	array[ONE ][i] = rand()%100;
    }

    printarray(array[ZERO]);
    i = mergesort_itr(array, ARR_SIZE);
    return printarray(array[i]);

}
#endif

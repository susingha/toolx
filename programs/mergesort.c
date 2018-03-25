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
void merge_(int arr1[], int size1, int arr2[], int size2, int res[]) {
    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2) {
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
    while (i < size1) {
	res[k] = arr1[i];
	i++;
	k++;
    }
    while (j < size2) {
	res[k] = arr2[j];
	j++;
	k++;
    }
}

// Prepares the 2D array A to be fed into the actual merge_ function
// returns an integer to identify which array to find the sorted array in
int merge(int A[TWO][ARR_SIZE], int lh, int rh, int low1, int high1, int low2, int high2) {
    int i = low1, ret;
    int *array1, *array2, *res;

    if (lh == rh) {
	array1 = array2 = A[lh];
    } else {
	array1 = A[lh];
	array2 = A[rh];
    }

    ret = 1-lh;
    res = A[ret];

    merge_(&array1[low1], high1-low1+1, &array2[low2], high2-low2+1, &res[low1]);

    return ret;
}

// Mergesort in O(n) space using 2 arrays.
// returns and integer to identify which array to find the sorted array in
int mergesort (int A[TWO][ARR_SIZE], int h, int low, int high) {
    int div;
    int lh, rh;

    if (low > high) {
	printf("FATAL: low: %d, high: %d\n", low, high);
        return h;
    }

    if (low == high) {
        return h;
    }

    div = (low + high)/2;
    lh = mergesort(A, h, low, div);
    rh = mergesort(A, h, div+1, high);

    return merge(A, lh, rh, low, div, div+1, high);
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
    i = mergesort(array, ZERO, 0, ARR_SIZE - 1);
    return printarray(array[i]);

}
#endif

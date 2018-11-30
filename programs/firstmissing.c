/*
 * Given an array of integers, find the first missing positive integer
 * in linear time and constant space.
 * In other words, find the lowest positive integer
 * that does not exist in the array.
 * The array can contain duplicates and negative numbers as well.
 * For example, the input [3, 4, -1, 1] should give 2. The input [1, 2, 0] should give 3.
 * You can modify the input array in-place.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "template.h"

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif


void showarr(int arr[], int n)
{
    int i = 0;
    while (n--) printf("%d, ", arr[i++]);
    printf("\n");
}

void firstmissing(int arr[], int len)
{
    int i = 0, min = 99999, num;;
    int debug = FALSE;

    showarr(arr, len);

    // Set all -ve numbers to 0
    for (i = 0; i < len; ++i) {
	if (arr[i] < 0) arr[i] = 0;
    }
    if (debug) showarr(arr, len);

    // Find the smallest number
    for (i = 0; i < len; ++i) {
	if (arr[i] && arr[i] < min) min = arr[i];
    }
    if (debug) printf("smallest number = %d\n", min);

    if (min == 99999) {
	printf("First missing: 1\n");
	return;
    }

    /* subtract min from all non zero */
    for (i = 0; i < len; ++i) {
	if (arr[i]) arr[i] -= min;
    }
    if (debug) showarr(arr, len);

    /* Add 1 to  all */
    for (i = 0; i < len; ++i) {
	arr[i]++;
    }
    if (debug) showarr(arr, len);

    /* use each number as index and mark those indices as negave (or present) */
    for (i = 0; i < len; ++i) {
	num = abs(arr[i]);
	if (num < len) {
	    arr[num] = 0 - abs(arr[num]);
	}
    }
    if (debug) showarr(arr, len);

    /* Find the first +ve number */
    for (i = 1; i < len; ++i) {
	if (arr[i] > 0) {
	    printf("First missing: %d\n", i-1+min);
	    return;
	}
    }
    printf("First missing: %d\n", i-1+min);

}

#define arrlen(x)  sizeof(x)/sizeof(x[0])

int main (int argc, char *argv[])
{
    int arr0[] = {0};
    int arr1[] = {-2, -1, 0, 1, 3, 4};
    int arr2[] = {3, 4, -1, 1};
    int arr3[] = {0, 1, 2};
    int arr4[] = {99, 100, 103};
    int arr5[] = {99, 100, 103, -1};
    int arr6[] = {99, 100, 103, -1, 1};
    int arr7[] = {0, 0, 0};
    int arr8[] = {-1, -1, -1};
    int arr9[] = {-1, -2, -3};

    firstmissing(arr0, arrlen(arr0));
    firstmissing(arr1, arrlen(arr1));
    firstmissing(arr2, arrlen(arr2));
    firstmissing(arr3, arrlen(arr3));
    firstmissing(arr4, arrlen(arr4));
    firstmissing(arr5, arrlen(arr5));
    firstmissing(arr6, arrlen(arr6));
    firstmissing(arr7, arrlen(arr7));
    firstmissing(arr8, arrlen(arr8));
    firstmissing(arr9, arrlen(arr9));

    return 0;
}


/*
 * A little more simpler approach
 * Group all positives towards left and all 0/-ve numbers to the right (pivot finding method)
 * We are only interested in the positives section
 * Reduce the positive numbers to 1 based by subtracting (minimum + 1)
 * Negave indeces as per the 1 based positive numbers
 * Find the first index with +ve number.
 8 Return index + min - 1
 */






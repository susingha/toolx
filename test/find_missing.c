#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int find(int * arr, int len) {

    int low = 0;
    int high = len - 1;
    int mid = 0;

    if (arr[low] + (high - low) == arr[high])
	return 0;

    while (low + 1 < high) {

	mid = (low + high) / 2;

	if (arr[low] + (mid - low) == arr[mid]) {
	    // continue in 2nd half
	    low = mid;
	} else {
	    // continue in 1st half
	    high = mid;
	}

    }

    return arr[low] + 1;
}


int main () {
    int arr[5][5] = {{4,5,6,7,8},   // no missing
	             {4,6,7,8,9},   //  5 missing
	             {4,5,7,8,9},   //  6 missing
	             {4,5,6,8,9},   //  7 missing
	             {4,5,6,7,9}};  //  8 missing

    int m = 99999;

    m = find(arr[0], 5);
    printf("sup: missing = %d\n", m);
    m = find(arr[1], 5);
    printf("sup: missing = %d\n", m);
    m = find(arr[2], 5);
    printf("sup: missing = %d\n", m);
    m = find(arr[3], 5);
    printf("sup: missing = %d\n", m);
    m = find(arr[4], 5);
    printf("sup: missing = %d\n", m);

    return 0;
}


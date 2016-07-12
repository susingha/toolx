#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int find(char * arr, int len) {

    if (arr[len - 1] == arr[0] + len - 1)
	return 0;

    while (1) {
	if (arr[0] == arr[(len/2) - 1])
    }

    return 0;
}


int main () {
    int arr[][] = {{4,5,6,7,8},   // no missing
	           {4,6,7,8,9},   //  5 missing
	           {4,5,7,8,9},   //  6 missing
	           {4,5,6,8,9},   //  7 missing
	           {4,5,6,7,9},   //  8 missing
                             };

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


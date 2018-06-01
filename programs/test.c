#include <stdio.h>
#include <stdlib.h>

int find(int arr[], int size) {
    int b = 0;
    int e = size - 1;
    int m = (b+e)/2;



    if (arr[b] + e == arr[b + e])
	return -1;

    if (size == 2) {
	return arr[b] + 1;
    }

    if (arr[b] + m == arr[b + m]) {
	// go right
	return find(&arr[m], e-b+1);
    } else {
	// go left
	return find(&arr[b], m-b+1);
    }

    return 0;

}



#define MAX 6
int main () {
    int arr[5][MAX] = {{4,5,6,7,8,9 },   // no missing
	              {4,6,7,8,9,10},   //  5 missing
	              {4,5,7,8,9,10},   //  6 missing
	              {4,5,6,8,9,10},   //  7 missing
	              {3,4,5,6,7,9}};   //  8 missing

    int m = 99999;

    m = find(arr[0], MAX);
    printf("sup: missing = %d\n", m);
    m = find(arr[1], MAX);
    printf("sup: missing = %d\n", m);
    m = find(arr[2], MAX);
    printf("sup: missing = %d\n", m);
    m = find(arr[3], MAX);
    printf("sup: missing = %d\n", m);
    m = find(arr[4], MAX);
    printf("sup: missing = %d\n", m);

    return 0;
}


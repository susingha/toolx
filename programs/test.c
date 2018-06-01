#include <stdio.h>
#include <stdlib.h>

void display(int arr[], int size) {
    int i = 0;
    for (i = 0; i < size; ++i) {
	printf("%2d, ", arr[i]);
    }
    printf("\n");
}


void merge(int arr1[], int len1,
           int arr2[], int len2,
           int arr3[]) {

    int i = 0, j = 0, k = 0;

    while (i < len1 && j < len2) {

	if (arr1[i] < arr2[j] || arr1[i] == arr2[j]) {
	    arr3[k] = arr1[i];
	    i++;
	    k++;
	} else {
	    arr3[k] = arr2[j];
	    j++;
	    k++;
	}
    }

    while (i < len1) {
	arr3[k] = arr1[i];
	i++;
	k++;
    }

    while (j < len2) {
	arr3[k] = arr2[j];
	j++;
	k++;
    }
}

void msort(int arr1[], int b, int e, int arr2[]) {
    int m;

    if (b >= e) {
	return;
    }

    m = (b + e)/2;
    msort(arr2, b,   m, arr1);
    msort(arr2, m+1, e, arr1);

    merge(&arr1[b],   m-b+1,
	  &arr1[m+1], e-(m+1)+1,
	  &arr2[b]);
}

#define MAX 20
int array1[MAX];
int array2[MAX];

int main () {
    int i = 0;
    int num = 0;
    srand(22);
    for (i = 0; i < MAX; ++i) {
	num = rand()%100;
	array1[i] = num;
	array2[i] = num;
    }

    display(array1, MAX);
    display(array2, MAX);

    msort(array1, 0, MAX-1, array2);
//  display(array1, MAX);
    display(array2, MAX);

}





/*
 * Given an array of integers,
 * return a new array such that
 * each element at index i of the new array
 * is the product of all the numbers in the original array
 * except the one at i.
 *
 * For example, if our input was [1, 2, 3, 4, 5],
 * the expected output would be [120, 60, 40, 30, 24].
 * If our input was [3, 2, 1],
 * the expected output would be [2, 3, 6].
 *
 * Dont use Division
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "template.h"



void getarr(int arr[], int n)
{
    while (n--) arr[n] = getrandom();
    printf("\n");
}

void showarr(int arr[], int n)
{
    int i = 0;
    while (n--) printf("%d, ", arr[i++]);
    printf("\n");
}



int arr1[1024];
int arr2[1024];
int len = 5;

int main (int argc, char *argv[])
{

    int product = 1;;
    getarr(arr1, len);
    getarr(arr2, len);

    showarr(arr1, len);

    int i;
    for (i = 0; i < len; ++i) {
	arr2[i] = 1;
    }

    product = 1;
    for (i = 1; i < len; ++i) {
	product = product * arr1[i - 1];
	arr2[i] = product;
    }

    product = 1;
    for (i = len-2; i >= 0; --i) {
	product = product * arr1[i + 1];
	arr2[i] = product * arr2[i];
    }

    showarr(arr2, len);

    return 0;
}




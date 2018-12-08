
/*
 * Given a list of integers,
 * write a function that returns the largest sum of non-adjacent numbers.
 * Numbers can be 0 or negative.
 * 
 * For example, [2, 4, 6, 2, 5] should return 13, since we pick 2, 6, and 5.
 * [5, 1, 1, 5] should return 10, since we pick 5 and 5.
 * 
 * Follow-up: Can you do this in O(N) time and constant space?
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "template.h"

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif


struct node {
    int num;
    char c;
    struct node *next, *prev; // used for linkedlist
    struct node *lptr, *rptr; // used for tree
};

void getarr(int arr[], int n)
{
    while (n--) arr[n] = getrandom();
}

void showarr(int arr[], int n)
{
    int i = 0;
    while (n--) printf("%d, ", arr[i++]);
    printf("\n");
}

int max (int a, int b)
{
    if(a > b)
	return a;
    else
	return b;
}


#if 0
int arr[] = {2,4,6,2,5};
int arr[] = {5,1,1,5};
int arr[] = {5,1,1,5,100};
int arr[] = {5,1,1,5,6};
#endif
int arr[] = {5,1,1,5};
int len;

int maxsum()
{
    int sumbut1 = 0;
    int sumbut2 = 0;
    int i = 0, sum1, sum2;
    for (i = 0; i < len; ++i) {

	// including
	sum1 = arr[i] + sumbut2;

	// excluding
	sum2 = sumbut1;

	sumbut2 = sumbut1;
	sumbut1 = max(sum1, sum2);;
    }

    return sumbut1;
}

int main (int argc, char *argv[])
{
    printargs(argc, argv);

    len = sizeof(arr)/sizeof(arr[0]);
    showarr(arr, len);

    printf("maxsum broken = %d\n", maxsum());

    return 0;
}




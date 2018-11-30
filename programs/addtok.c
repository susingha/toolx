/*
 * Given a list of numbers and a number k,
 * return whether any two numbers from the list add up to k.
 * 
 * For example, given [10, 15, 3, 7] and k of 17,
 * return true since 10 + 7 is 17.
 * 
 * TODO: Can you do this in one pass?
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "template.h"


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


void swap(int arr[], int a, int b)
{
    int t = arr[a];
    arr[a] = arr[b];
    arr[b] = t;
}

void quicksort(int arr[], int len)
{
    int hold = 0, probe = 1;
    int pivi = rand() % len;
    int piv  = arr[pivi];

    swap(arr, 0, pivi);
    pivi = 0;

    hold = 1;
    probe = 1;
    while (probe < len) {
	if (arr[probe] <= piv) {
	    swap(arr, hold, probe);
	    hold++;
	    probe++;
	} else {
	    probe++;
	}
    }
    swap(arr, pivi, hold-1);
    if (hold-1 > 0)
	quicksort(&arr[0], hold-1);
    if (probe-hold > 0)
	quicksort(&arr[hold], probe-hold);
}

int addtok(int sum, int arr[], int len)
{
    int rem = 0;
    int bptr = 0,
	eptr = len-1;

    int ret = FALSE;
    while (bptr < eptr) {
	rem = sum - arr[bptr];
	while (arr[eptr] > rem)
	    eptr--;
	if (arr[bptr] + arr[eptr] == sum && bptr != eptr) {
	    printf("%d + %d = %d\n", arr[bptr], arr[eptr],  sum);
	    ret = TRUE;
	}
	bptr++;
    }

    return ret;
}

int arr[1024];
int len = 40;

int main (int argc, char *argv[])
{
    getarr(arr, len);
    int sum = rand()%150;
    printf("sum: %d\n", sum);

    trackon();
    quicksort(arr, len);
    trackoff();

    showarr(arr, len);

    trackon();
    addtok(sum, arr, len);
    trackoff();

    trackprint();

    return 0;
}




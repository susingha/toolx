/*
 * Given a list of numbers and a number k,
 * return whether any two numbers from the list add up to k.
 * 
 * For example, given [10, 15, 3, 7] and k of 17,
 * return true since 10 + 7 is 17.
 * 
 * TODO: Can you do this in one pass?i
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif

#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

#include <time.h>
#ifdef MEASURE_RUNTIME
clock_t ts;
#endif

#define RANDOM
#ifdef RANDOM
#define rnd() (rand()%100)
#else
#define rnd() (dataset[dataindex++])
#endif

int * getarr(int n)
{
    int * arr = calloc(n, sizeof(int));
    srand(time(NULL));
    while (n--) {
	arr[n] = rnd();
	printf("%d, ", arr[n]);
    }
    printf("\n");
    return arr;
}

void shoarr(int arr[], int n)
{
    while (n--) {
	printf("%d, ", arr[n]);
    }
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
    int pivi = rnd() % len;
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


int main (int argc, char *argv[])
{
    int c;
    for (c = 0; c < argc; ++c) {
	printf("%s ", argv[c]);
    }
    printf("(%d)\n", argc);

#ifdef MEASURE_RUNTIME
    ts = clock();
    unsigned int i = 0, j;
    j = --i;
    while(--i);
    ts = clock() - ts;
    printf("Counted %u to %u in %f seconds\n", j, i, ((double)ts) / CLOCKS_PER_SEC); 
#endif

    int len = 40;
    int * arr = getarr(len);
    int sum = rand()%150;
    printf("sum: %d\n", sum);

    quicksort(arr, len);
    shoarr(arr, len);

    addtok(sum, arr, len);

    free(arr);
    return 0;
}




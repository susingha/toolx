/*
 * Given a set of integers check if any combination of addition / subtraction
 * sums up to a given total
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

#define MEASURE_RUNTIME
#ifdef MEASURE_RUNTIME
#include <time.h>
clock_t ts;
#endif

#define RANDOM
#ifdef RANDOM
#define rnd() (rand()%100)
#else
#define rnd() (dataset[dataindex++])
#endif

char strbuild[1024];

int comRest(int arr[], int idx, int n, int runsum, int fullsum, int stridx) {
    int res = 0, offset = 0, res1 = 0, res2 = 0;

    if (idx == n) {
	if (runsum == fullsum) {
	    strbuild[stridx] = ENDS;
	    // printf("%s= %d\n", strbuild, runsum);
	    return TRUE;
	}

	strbuild[stridx] = 0;
	return FALSE;
    }


    offset = sprintf(&strbuild[stridx], "+ %d ", arr[idx]);
    res1 = comRest(arr, idx+1, n, runsum + arr[idx], fullsum, stridx + offset);

    offset = sprintf(&strbuild[stridx], "- %d ", arr[idx]);
    res2 = comRest(arr, idx+1, n, runsum - arr[idx], fullsum, stridx + offset);

    return res1 | res2;
}

int * getarr(int n) {
    int * arr = calloc(n, sizeof(int));
    srand(time(NULL));
    while (n--) {
	arr[n] = rnd();
	printf("%d, ", arr[n]);
    }
    printf("\n");
    return arr;
}

int main (int argc, char *argv[])
{
    int c, res;
    int total = 50;
    int arrlen = atoi(argv[1]);
    int * arr = getarr(arrlen);

    printf("%d elements\n", arrlen);

    ts = clock();
    res = comRest(arr, 0, arrlen, 0, total, 0);
    ts = clock() - ts;

    if(res == TRUE) {
	printf("True\n");
    } else {
	printf("False\n");
    }

    printf("Elapsed %f seconds\n", ((double)ts) / CLOCKS_PER_SEC); 
    return 0;
}




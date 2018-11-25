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

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif

#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

#ifdef MEASURE_RUNTIME
clock_t _ts_;
#endif

#define RANDOM
#ifdef RANDOM
#define rnd() (rand()%10)
#else
#define rnd() (dataset[dataindex++])
#endif

int * getarr(int n) {
    int * arr = calloc(n, sizeof(int));
    srand(time(NULL));
    while (n--) {
	arr[n] = n+1;
    }
    printf("\n");
    return arr;
}

void showarr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i) {
	printf("%d, ", arr[i]);
    }
    printf("\n");
}

int main (int argc, char *argv[])
{
    int _c_;
    for (_c_ = 0; _c_ < argc; ++_c_) {
	printf("%s ", argv[_c_]);
    }
    printf("(%d)\n", argc);


#ifdef MEASURE_RUNTIME
    _ts_ = clock();
    unsigned int _i_ = 0, _j_;
#endif


    int len = 5, product = 1;;
    int * arr1 = getarr(len);
    int * arr2 = getarr(len);

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

    free(arr2);
    free(arr1);



#ifdef MEASURE_RUNTIME
    _ts_ = clock() - _ts_;
    printf("Counted %u to %u in %f seconds\n", _j_, _i_, ((double)_ts_) / CLOCKS_PER_SEC); 
#endif
    return 0;
}




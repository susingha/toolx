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

#define MEASURE_RUNTIME
#ifdef MEASURE_RUNTIME
clock_t _ts_;
#endif

#define RANDOM
#ifdef RANDOM
#define rnd() (rand()%100)
#else
#define rnd() (dataset[dataindex++])
#endif

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
    int _c_;
    for (_c_ = 0; _c_ < argc; ++_c_) {
	printf("%s ", argv[_c_]);
    }
    printf("(%d)\n", argc);

#ifdef MEASURE_RUNTIME
    _ts_ = clock();
    unsigned int _i_ = 0, _j_;
    _j_ = --_i_;
    while(--_i_);
    _ts_ = clock() - _ts_;
    printf("Counted %u to %u in %f seconds\n", _j_, _i_, ((double)_ts_) / CLOCKS_PER_SEC); 
#endif

    int * arr = getarr(10);

    free(arr);
    return 0;
}




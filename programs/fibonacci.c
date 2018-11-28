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




#define MAX 50
int fibcache[MAX];
#define USECACHE          // sup: use Dynamic Programming

void showcache()
{
    int i;
    for (i = 0; i < MAX; ++i) {
	printf("%d, ", fibcache[i]);
    }
    printf("\n");
}

int fib(int n)
{
    int nm1, nm2;

#ifdef USECACHE
    if (fibcache[n])
	return fibcache[n];
#endif

    if (n == 1 || n == 0) {
	fibcache[n] = 1;
	showcache();
	return 1;
    }

    nm2 = fib(n-2);
    nm1 = fib(n-1);

    fibcache[n] = nm1 + nm2;
    showcache();
    return nm1 + nm2;
}


int main (int argc, char *argv[])
{
    int _c_;
    for (_c_ = 0; _c_ < argc; ++_c_) {
	printf("%s ", argv[_c_]);
    }
    printf("(%d)\n", argc);





    int fibn = 15;
    int i;

    if (fibn > MAX) {
	printf("Limit fib to 50. Or change MAX\n");
	return 2;
    }

    for (i = 0; i < MAX; ++i)
	fibcache[i] = 0;

#ifdef MEASURE_RUNTIME
    _ts_ = clock();
#endif

    printf("%d Fibonacci number is: %d\n", fibn, fib(fibn));

#ifdef MEASURE_RUNTIME
    _ts_ = clock() - _ts_;
    printf("Time: %f seconds\n", ((double)_ts_) / CLOCKS_PER_SEC); 
#endif
    return 0;
}




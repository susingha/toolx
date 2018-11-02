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

    int * arr = getarr(10);

    free(arr);
    return 0;
}





/*
 * There's a staircase with N steps, and you can climb 1 or 2 steps at a time.
 * Given N, write a function that returns the number of unique ways you can
 * climb the staircase. The order of the steps matters.
 * For example, if N is 4, then there are 5 unique ways:
 * - 1, 1, 1, 1
 * - 2, 1, 1
 * - 1, 2, 1
 * - 1, 1, 2
 * - 2, 2
 * What if, instead of being able to climb 1 or 2 steps at a time,
 * you could climb any number from a set of positive integers X?
 * For example, if X = {1, 3, 5}, you could climb 1, 3, or 5 steps at a time.
 * Generalize your function to take in X.
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

#define MEASURE_RUNTIME
#ifdef MEASURE_RUNTIME
#include <time.h>
clock_t _ts_;
#endif

#define RANDOM
#ifdef RANDOM
#define rnd() (rand()%100)
#else
#define rnd() (dataset[dataindex++])
#endif


struct node {
    int         * steps;
    struct node * stepslist;
};


#define FULLSUM (4)

int sum = FULLSUM;
int steps[] = {1,2};
int stepslen = sizeof(steps) / sizeof(steps[0]);

int results[FULLSUM];
int totalresults = 0;

void printresult(int reslen)
{
    int i, verifysum = 0;
    for(i = 0; i < reslen; ++i) {
	printf("%d, ", results[i]);
	verifysum += results[i];
    }
    if (verifysum != sum) {
	printf("Answer is WRONG: total: %d, expected: %d", verifysum, sum);
    } else {
	totalresults++;
    }
    printf("\n");
}

void printseries(int localprevsum, int resultsj, int stepsbeg)
{

    int i = stepsbeg; // i will index the steps array
    int j = resultsj; // i will index the results array
    int localsum;

    for (i = stepsbeg; i < stepslen; ++i) {

	results[j] = steps[i];
	localsum = localprevsum + results[j];

	if (localsum == sum) {
	    printresult(j+1);

	} else if (localsum < sum /* && i < stepslen */) {
	    printseries(localsum, j+1, 0);

	} else if (localsum > sum) {
	    continue; // do nothing

	}
    }

    return;
}

int main (int argc, char *argv[])
{
#ifdef MEASURE_RUNTIME
    _ts_ = clock();
    unsigned int _i_ = 0, _j_;
#endif

    int i = 0;
    printf("sum = %d\n", sum);
    for (i = 0; i < stepslen; ++i) {
	printf("%d, ", steps[i]);
    }
    printf("\n========================\n");

    printseries(0, 0, 0);
    printf("Total answers: %d\n", totalresults);

#ifdef MEASURE_RUNTIME
    _ts_ = clock() - _ts_;
    printf("Counted %u to %u in %f seconds\n", _j_, _i_, ((double)_ts_) / CLOCKS_PER_SEC); 
#endif
    return 0;
}




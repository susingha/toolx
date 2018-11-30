#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "template.h"

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
    int fibn = 15;
    int i;

    if (fibn > MAX) {
	printf("Limit fib to 50. Or change MAX\n");
	return 2;
    }

    for (i = 0; i < MAX; ++i)
	fibcache[i] = 0;

    trackon();

    printf("%d Fibonacci number is: %d\n", fibn, fib(fibn));

    trackoff(); trackprint();

    return 0;
}




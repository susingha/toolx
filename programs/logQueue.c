
/*
 * You run an e-commerce website and want to record the last N order ids in a log.
 * Implement a data structure to accomplish this, with the following API:
 * 
 * record(order_id): adds the order_id to the log
 * get_last(i): gets the ith last element from the log.
 * i is guaranteed to be smaller than or equal to N.
 * You should be as efficient with time and space as possible.
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



int logarr[10];
int len = sizeof(logarr)/sizeof(logarr[0]);
int wptr = 0;
int bptr = -1;

int inc(int k, int p)
{
    return (k + p) % len;
}

int dec(int k, int p)
{
    k = k - p;

    if (k < 0)
	return len + k;
    return k;
}

void record(int logid)
{
    printf("%s %d\n", __FUNCTION__, logid);

    logarr[wptr] = logid;

    if (bptr == wptr || bptr == -1)
	bptr = inc(bptr, 1);

    wptr = inc(wptr, 1);
}

void getlast(int i)
{
    int pi = dec(wptr, 1);
    int count = 0;

    printf("%s %d\n", __FUNCTION__, i);

    if (bptr == -1) {
	printf("\tno orders\n");
	return;
    }

    if (i <= 0) {
	return;
    }

    while(TRUE) {
	printf("\trecent order id: %d\n", logarr[pi]);
	count++;

	if (pi == bptr || count == i)
	    break;
	else
	    pi = dec(pi, 1);
    }
}

int main (int argc, char *argv[])
{
    printargs(argc, argv);

    for (int i = 0; i < len; ++i)
	logarr[i] = 0;


    getlast(0);
    getlast(1);

    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    getlast(0);
    getlast(1);
    getlast(5);
    getlast(-5);
    getlast(15);
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    record(getrandom());
    getlast(0);
    getlast(1);
    getlast(5);
    getlast(-5);
    getlast(10);
    getlast(15);

    return 0;
}




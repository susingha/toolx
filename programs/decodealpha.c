#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "template.h"

/*
 *  Given the mapping a = 1, b = 2, ... z = 26, and an encoded message,
 *  count the number of ways it can be decoded.
 *
 *  For example, the message '111' would give 3,
 *  since it could be decoded as 'aaa', 'ka', and 'ak'.
 *
 *  You can assume that the messages are decodable.
 *  For example, '001' is not allowed.
 */


#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif


struct node {
    int num;
    char c;
    struct node *next, *prev; // used for linkedlist
    struct node *lptr, *rptr; // used for tree
};

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


char charat(int i)
{
    return 'a' + i - 1;
}

char * strin;
int strln, maxidx;

int getnum (int idx1, int idx2)
{
    int i1 = strin[idx1] - '1' + 1;;
    int i2 = strin[idx2] - '1' + 1;;
    return  ((i1 * 10 ) + i2);
}

int countdecodes (int idx)
{
    int ret = 0;
    int num = 0;
    int groupOneCount = 0;
    int groupTwoCount = 0;

    if (idx > maxidx) {
	return 0;
    }

    if (idx == maxidx) {
	return 1;
    }

    if (idx + 1 <= maxidx) {
	groupOneCount = countdecodes(idx + 1);
    }

    // At this point idx and idx + 1 is valid

    num = getnum(idx, idx+1);
    if (num < 1 || 26 < num) {
	return groupOneCount;
    }

    if (idx + 2 <= maxidx) {
	groupTwoCount = countdecodes(idx + 2);
    } else {
	groupTwoCount = 1;
    }

    return groupOneCount + groupTwoCount;
}


int main (int argc, char *argv[])
{
    printargs(argc, argv);

    if (argc < 2) {
	printf("Specify a series of numbers like: 1213425\n");
	return -1;
    }

    strin = argv[1];
    strln = strlen(strin);
    maxidx = strln - 1;

    trackon();

    printf("Possible strings: %d\n", countdecodes(0));

    trackoff(); trackprint();

    return 0;
}




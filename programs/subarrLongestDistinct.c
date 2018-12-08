
/*
 * Given an integer k and a string s,
 * find the length of the longest substring that contains at most k distinct characters.
 * 
 * For example, given s = "abcba" and k = 2,
 * the longest substring with k distinct characters is "bcb".
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "template.h"

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif

#if 0
int k = 2; char str[] = "abcba";
int k = 3; char str[] = "pabcbcpa";
int k = 3; char str[] = "ppbbbbpa";
int k = 2; char str[] = "ppbbbbpa";
#endif
int k = 2; char str[] = "abcba";

int hash[26];
#define INC 1
#define DEC 0

int getdistcnt(int b, int e, int distcnt, int change)
{
    switch (change) {
	case INC:
	    if((hash[str[e]]++) == 0)
		distcnt++;
	    break;
	case DEC:
	    if((--hash[str[b-1]]) == 0)
		distcnt--;
	    break;
	default:
	    break;
    }

    return distcnt;
}

int main (int argc, char *argv[])
{
    for (int i = 0; i < 26; ++i)
	hash[i] = 0;

    int maxidx = strlen(str) - 1;

    int  b = 0,  e = 0;
    int ib = b, ie = e;
    int distcnt = getdistcnt(b, e, 0, INC);
    int maxlen = e-b+1, gap = e-b+1;
    int change;

    while (TRUE) {

	if (distcnt <= k) {
	    e++;
	    change = INC;
	} else {
	    b++;
	    change = DEC;
	}

	gap = e-b+1;

	if (e > maxidx)
	    break;

	if (e == maxidx && gap <= maxlen)
	    break;

	distcnt = getdistcnt(b, e, distcnt, change);
	if (distcnt <= k && gap > maxlen) {
	    maxlen = gap;
	    ib = b;
	    ie = e;
	}
    }

    printf("%s <- Input\n", str);
    for (int j = 0; j <= maxidx; ++j) {
	if (j == ib)
	    printf("[");
	printf("%c", str[j]);
	if (j == ie)
	    printf("]");
    }
    printf(" <- Output\n");
    printf("maxlen with %d distinct characters is %d. from index %d to %d\n", k, maxlen, ib, ie);

    return 0;
}




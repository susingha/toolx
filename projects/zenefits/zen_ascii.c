#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "zen_types.h"

/*
   ASCII Mappings:
   0 - 0
   1 - 1
   .
   .
   9 - 9
   a - 10
   b - 11
   .
   .
   z - 35
   - - 36
   . - 37
 
 */

#define MAXQUERYLEN 100
int ascix_map[CHARMAX];
int ascix_init = FALSE;

int ascix_setup()
{
    int i = 0, j = 0;

    // counting max number of children per trie node
    // for all possible chars in a domain name

    for(j = 0; j < CHARMAX; ++j)
	ascix_map[j] = -1;

    ascix_map['0'] = i++;
    ascix_map['1'] = i++;
    ascix_map['2'] = i++;
    ascix_map['3'] = i++;
    ascix_map['4'] = i++;
    ascix_map['5'] = i++;
    ascix_map['6'] = i++;
    ascix_map['7'] = i++;
    ascix_map['8'] = i++;
    ascix_map['9'] = i++;
    ascix_map['a'] = i++;
    ascix_map['b'] = i++;
    ascix_map['c'] = i++;
    ascix_map['d'] = i++;
    ascix_map['e'] = i++;
    ascix_map['f'] = i++;
    ascix_map['g'] = i++;
    ascix_map['h'] = i++;
    ascix_map['i'] = i++;
    ascix_map['j'] = i++;
    ascix_map['k'] = i++;
    ascix_map['l'] = i++;
    ascix_map['m'] = i++;
    ascix_map['n'] = i++;
    ascix_map['o'] = i++;
    ascix_map['p'] = i++;
    ascix_map['q'] = i++;
    ascix_map['r'] = i++;
    ascix_map['s'] = i++;
    ascix_map['t'] = i++;
    ascix_map['u'] = i++;
    ascix_map['v'] = i++;
    ascix_map['w'] = i++;
    ascix_map['x'] = i++;
    ascix_map['y'] = i++;
    ascix_map['z'] = i++;
    ascix_map['-'] = i++;
    ascix_map['.'] = i++;

    // there will be 38 children max not 255

    return i;
}

int ascix(char c)
{
    int i = 0, mapped_ascii;

    if (!ascix_init) {
	ascix_setup();
	ascix_init = TRUE;
    }

    mapped_ascii = ascix_map[c];

    if (mapped_ascii == -1) {
	printf("illegal character: %c [%u] in input\n", c, c);
	exit(2);
    }

    return mapped_ascii;
}

int sanitize(char query[], int n) {
    int i = 0, mapped_ascii;
    char c;

    if (n > MAXQUERYLEN)
	return FALSE;

    for (i = 0; i < n; ++i) {

	c = query[i];
	mapped_ascii = ascix_map[c];

	if (mapped_ascii == -1)
	    return FALSE;
    }

    return TRUE;
}


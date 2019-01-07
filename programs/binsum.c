
// Sum of two signed numbers without using + or - sign
// Use a Half Adder method to get a MUCH simpler implementation

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "template.h"

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


int getbitat(int num, int idx)
{
    return (num >> idx) & 0x1;
}
int setbitat(int num, int idx, int val)
{
    if (val)
	num |= 0x1 << idx;
    else
	num &= ~(0x1 << idx);

    return num;
}

int add(int a, int b)
{
    int result = 0;
    int len = sizeof(int) << 3;
    int i = 0;

    int carry = 0;
    int abit = 0, bbit = 0, rbit = 0;

    for (i = 0; i < len; ++i) {
	abit = getbitat(a, i);
	bbit = getbitat(b, i);

#if 1
	if (carry) {
	    carry = 0;
	    rbit = 0;
	    if (abit | bbit)
		carry = 1;
	    if (abit == bbit)
		rbit = 1;
	} else {
	    carry = 0;
	    rbit = abit | bbit;
	    if (abit && bbit) {
		carry = 1;
		rbit = 0;
	    }
	}
#endif

	result = setbitat(result, i, rbit);
    }

    return result;
}

int main (int argc, char *argv[])
{
    printargs(argc, argv);
    if (argc != 3) {
	printf("specify two numbers to add\n");
	return -1;
    }

    int a = atoi (argv[1]);
    int b = atoi (argv[2]);
    printf("adding %d + %d = %d\n", a, b, add(a, b));

    return 0;
}




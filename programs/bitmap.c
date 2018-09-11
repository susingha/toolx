#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#if 0
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#endif

#if 0
To be added:
	How to   set a bit or nth bit
	How to unset a bit or nth bit
	How to count the number of bits
	How to print binary
	How to reverse the bit-pattern of a number
	How to unset the  most significant bit of a number
	How to unset the least significant bit of a number
	How to check for little endian and bit endian
#endif

int binary(int num) {
    return 0;
}

int count1s(int num) {
    int i = 0;
    int n = num;
    while (n) {
	n = n & (n-1);
	i++;
    }

    printf("%d (0x%x) has %d 1s\n", num, num, i);
    return i;
}


int main () {

    count1s(1);
    count1s(2);
    count1s(3);
    count1s(5);
    count1s(1023);

    return 0;
}




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "template.h"


int main (int argc, char *argv[])
{
    printargs(argc, argv);

    unsigned int i = 0, *ip;
    unsigned int j = 0x12345678;

    unsigned char c, *cp;

    ip = &i;
    printf("writing %X at %p\n", j, ip);
    *ip = j;

    if (*((unsigned char *)(ip)) == 0x12) // The    big end (msb) is written to the lowest memory
	printf("Big Endian\n");
    
    if (*((unsigned char *)(ip)) == 0x78) // the little end (lsb) is written to te lowest memory
	printf("Little Endian\n");

    cp = (unsigned char *)(ip) + 0;
    printf("value at address: %p: %x\n", cp, *cp);

    cp = (unsigned char *)(ip) + 1;
    printf("value at address: %p: %x\n", cp, *cp);

    cp = (unsigned char *)(ip) + 2;
    printf("value at address: %p: %x\n", cp, *cp);

    cp = (unsigned char *)(ip) + 3;
    printf("value at address: %p: %x\n", cp, *cp);

    return 0;
}




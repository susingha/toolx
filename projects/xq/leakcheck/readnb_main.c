/*
   You have been given a function called read4k(). This function reads 4K bytes from a file and returns the number of bytes read.
   You this function to create a file reading wrapper called readnb() that reads n bytes from a file and returns the number of bytes it could read.
   n has no correlation to 4K.

   */

#include <stdio.h>
#include <stdlib.h>
#include "read4k.h"

#define xq_printf ; // printf
#define FILENAME "./testtext.txt"

#define N4K          4096
#define N2K          2048
#define N1K          1024
#define N512         512
#define N128         128
#define N64          64
#define N32          32
#define N16          16
#define N8           8
#define N1           1
#define N0           0
#define NMORE        (BUF_4K + 1)
#define NLESS        (BUF_4K - 1)
#define NEQAL        (BUF_4K)

unsigned int readnb(unsigned int n, FILE * fp, char * copybuf);

int main () {

    unsigned int read_bytes = 0;
    FILE * fp = fopen(FILENAME, "r");
    char * copybuf = (char *)malloc(BUF_4K);

#if 0
    do {
	read_bytes = read4k(fp, copybuf);
	printf("sup: bytes read = %d\n", read_bytes);
    } while(read_bytes);
#else
    read_bytes = readnb(N4K-1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N4K,     fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N4K+1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N2K-1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N2K,     fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N2K+1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N1K-1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N1K,     fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N1K+1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N512-1,  fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N512,    fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N512+1,  fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N128-1,  fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N128,    fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N128+1,  fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N64-1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N64,     fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N64+1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N32-1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N32,     fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N32+1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N16-1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N16,     fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N16+1,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N8-1,    fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N8,      fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N8+1,    fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N1,      fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(N0,      fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(NMORE,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(NLESS,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
    read_bytes = readnb(NEQAL,   fp, copybuf); printf("sup: read = %u\n", read_bytes);
#endif

    return 0;
}


unsigned int readnb(unsigned int n, FILE * fp, char * copybuf) {

    unsigned int read = 0, read_bytes = 0;
    unsigned int remaining = n;

    if(!n)
	return 0;
    if(!fp)
	return 0;
    if(!copybuf)
	return 0;

    rewind(fp); // rewind() function sets the file position indicator to the beginning of the file.

    do {

	read_bytes = read4k(fp, copybuf);

	if(read_bytes == remaining) {
	    // we are done. we dont need to come back here.
	    // All read_bytes / remaining data are valid. Print the whole thing.
	    xq_printf("%.*s", remaining, copybuf);
	    read = read + remaining;
	    // update remaining = remaining - read_bytes
	    remaining = remaining - read_bytes; // same as remaining = 0;
	    // dont loop. exit
	} else if (read_bytes < remaining) {
	    // there is more to read. we need to come back here.
	    // All read_bytes data are valid. Print read_bytes number of data
	    xq_printf("%.*s", read_bytes, copybuf);
	    read = read + read_bytes;
	    // update remaining = remaining - read_bytes
	    remaining = remaining - read_bytes;
	    // loop
	} else if(read_bytes > remaining) {
	    // we read more than we needed to. we dont need to come back here
	    // Only remaining bytes is valid. Just print the remaining number of bytes
	    xq_printf("%.*s", remaining, copybuf);
	    read = read + remaining;
	    // remaining = 0
	    remaining = 0;
	    // dont loop. exit
	}

    } while(remaining && read_bytes);

    return read;

}









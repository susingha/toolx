#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/* Implement function readnb using given function read4k */

#define BUF_4K 128 // 4098
#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

unsigned int read4k(FILE * fp, void * buf4k_p) {
    if(!fp)
	return 0;
    if(!buf4k_p)
	return 0;

    return fread(buf4k_p, 1, BUF_4K, fp);
}

int min(int a, int b) {
    return a < b?a:b;
}


int readnb_1(FILE * fp, void * buff_n, unsigned int n) {

    int valid = 0, read = 0;
    int unreadbuf = n, readbuf = 0;
    void * buf4k_p = (void *)malloc(BUF_4K);

    if (!fp)
	return 0;
    if (!buff_n)
	return 0;
    if (!n)
	return 0;


    do {
	read = read4k(fp, buf4k_p);

	if (unreadbuf >= BUF_4K) {


	    memcpy(buff_n + readbuf, buf4k_p, read);
	    readbuf   = readbuf + read;
	    unreadbuf = unreadbuf - read;

	    if (read == BUF_4K)
		continue;
	    else
		break;

	} else {


	    valid = min(unreadbuf, read);

	    memcpy(buff_n + readbuf, buf4k_p, valid);
	    readbuf = readbuf + valid;
	    unreadbuf = 0;

	    break;
	}
    } while (1);

    *(char *)(buff_n + readbuf) = ENDS;
    return readbuf;
}

int readnb_2(FILE * fp, void * buff_n, unsigned int n) {

    void * buf4k_p = (void *)malloc(BUF_4K);

    int times = n / BUF_4K;
    int remdr = n % BUF_4K;

    int i = 0;
    int widx = 0, read = 0;
    int eof = FALSE;

    for (i = 0; i < times; ++i) {
	read = read4k(fp, buf4k_p);
	memcpy(buff_n + widx, buf4k_p, read);
	widx += read;

	if (read < BUF_4K) {
	    eof = TRUE;
	    break;
	}
    }

    if (!eof) {
	read = read4k(fp, buf4k_p);
	memcpy(buff_n + widx, buf4k_p, min(read, remdr));
	widx += min(read, remdr);
    }

    *(char *)(buff_n + widx) = ENDS;
    return widx;
}


int main (int argc, char *argv[]) {
    char * filename = NULL;
    int    bytes = 0, bytesread1 = 0, bytesread2 = 0;
    FILE * fp = NULL;
    void * buffn1 = NULL;
    void * buffn2 = NULL;
    char arr[] = "abcdefgh";

    if (argc > 2) {
	filename = argv[1];
	bytes = atoi(argv[2]);
	printf("Reading %d bytes from %s\n", bytes, filename);
    } else {
	printf("Arguments required: filename, bytes to read\n");
	return 0;
    }

    fp = fopen(filename, "r");
    buffn1 = (void *)malloc(bytes + 1);
    buffn2 = (void *)malloc(bytes + 1);

    if (!fp) {
	printf("Could not open file: %s\n", filename);
	return 1;
    }

    bytesread1 = readnb_1(fp, buffn1, bytes); rewind(fp); // old
    bytesread2 = readnb_2(fp, buffn2, bytes);             // new

    fclose(fp);

    printf("%s\n", (char *) buffn1);
    printf("%s\n", (char *) buffn2);

    printf("Read %d, bytes from %s\n", bytesread1, filename);
    printf("Read %d, bytes from %s\n", bytesread2, filename);
    if (bytesread1 != bytesread2) {
	printf("Read wrong bytes: %d, %d\n", bytesread1, bytesread2);
    }

   return 0;
}

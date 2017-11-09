#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_4K 128 // 4098

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

int readnb(FILE * fp, void * buff_n, unsigned int n) {

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

    *(char *)(buff_n + readbuf) = '\n';
    return readbuf;
}


int main (int argc, char *argv[]) {
    char * filename = NULL;
    int bytes = 0, bytesread = 0;
    FILE * fp = NULL;
    void * buffn = NULL;

    if (argc > 2) {
	filename = argv[1];
	bytes = atoi(argv[2]);
	printf("Reading %d bytes from %s\n", bytes, filename);
    } else {
	printf("Arguments required: filename, bytes to read\n");
	return 0;
    }

    fp = fopen(filename, "r");
    buffn = (void *)malloc(bytes);

    if (!fp) {
	printf("Could not open file: %s\n", filename);
	return 1;
    }

    bytesread = readnb(fp, buffn, bytes);
    fclose(fp);

    printf("Read %d bytes from %s\n", bytesread, filename);
    printf("%s\n", (char *)buffn);

    return 0;
}

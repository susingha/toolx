#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]) {
    char f1[1024], f2[1024];
    char * filename1 = NULL;
    char * filename2 = NULL;
    FILE * fp1 = NULL;
    FILE * fp2 = NULL;
    int bytes;
    char c, d;

    if (argc < 3) {
	printf("Usage: %s <file-to-read> <file-to-write>\n", argv[0]);
	return argc;
    }

    filename1 = argv[1];
    filename2 = argv[2];
    printf("Reading file %s\n", filename1);
    printf("Writing file %s\n", filename2);

    fp1 = fopen(filename1, "r");
    fp2 = fopen(filename2, "w");

    if (!fp1) {
	printf("cannot open %s for read\n", filename1);
	return -1;
    }
    if (!fp2) {
	printf("cannot open %s for write\n", filename2);
	return -2;
    }


    while ((c = fgetc(fp1)) != EOF) {
	bytes++;
	printf("%c", c);

	if ((d = fputc(c, fp2)) == EOF) {
	    printf("write error to file %s\n", filename2);
	    return -3;
	}
    }

    fclose(fp1);
    fclose(fp2);
    printf("Copied %d bytes from %s to %s\n", bytes, filename1, filename2);


    // Check md5sum
    sprintf(f1, "shasum %s", filename1);
    sprintf(f2, "shasum %s", filename2);
    system(f1);
    system(f2);

    return 0;
}



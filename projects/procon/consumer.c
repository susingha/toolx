#include <stdio.h>
#include <string.h>
#include "sharedmem.h"
#include "sh_sync.h"


char dstfile[] = "dstfile.txt";

char consume(char c, FILE *fp) {
    return fputc(c, fp);
}

int main() {
    FILE * fp = NULL;
    char string[1024];
    char * s = NULL;

    // init the consumer
    if ((fp = fopen(dstfile, "w")) == NULL) {
	perror("fopen");
	return -1;
    }

    // init the shared memory
    s = get_sharedmem(SHMKEY, SHMSIZE);
    if (s) printf("consumer: shared memory was obtained at %p\n", s);

    // start consuming


    return 0;
}






#ifdef SUP

read()
{
 
    s = shm;

    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');
 
    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*';
 
    exit(0);
}

#endif

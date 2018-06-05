#include <stdio.h>
#include <string.h>
#include "types_global.h"
#include "sharedmem.h"
#include "sh_sync.h"

char srcfile[] = "srcfile.txt";

char produce(FILE *fp) {
    return fgetc(fp);
}

int main() {
    FILE * fp = NULL;
    char c;
    int offset = -1;
    shbuf_t * s = NULL;

    // init the producer
    if ((fp = fopen(srcfile, "r")) == NULL) {
	perror("fopen");
	return -1;
    }

    // init the shared memory
    s = (shbuf_t *)get_sharedmem(SHMKEY, SHMSIZE);
    if (s) printf("producer: shared memory was obtained at %p\n", s);


    // start producing
    printf("Starting write:\n");
    while ((c = produce(fp)) != EOF) {
	printf("%c", c);
    }

    return 0;
}



#ifdef SUP

write()
{

    s = shm;

    for (c = 'a'; c <= 'z'; c++)
	*s++ = c;
    *s = NULL;

    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    while (*shm != '*')
	sleep(1);

    exit(0);
}

#endif

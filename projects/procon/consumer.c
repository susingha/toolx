#include <stdio.h>
#include "sharedmem.h"
#include "sh_sync.h"







int main() {
    int begin = 0, end = 0;
    char * s = NULL;
    s = get_sharedmem(SHKEY, SHMSZ);
    if (s) {
	printf("consumer: shared memory was obtained at %p\n", s);
    }





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

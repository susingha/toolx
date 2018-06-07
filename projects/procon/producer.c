#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "types_global.h"
#include "sem.h"
#include "sharedmem.h"
#include "shbuf.h"

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
    s = (shbuf_t *) get_sharedmem(SHMKEY, SHMSIZE);
    if (s) printf("producer: shared memory was obtained at %p\n", s);

    // init the shmem semaphore
    sem_t *bufferfull = sem_open(BUFFERFULL_SNAME, STARTUP_SFLAGS, STARTUP_SPERMS, 0);
    sem_t *bufferempt = sem_open(BUFFEREMPT_SNAME, STARTUP_SFLAGS, STARTUP_SPERMS, SHMBUFSiZE);
    // init the mutex in the form of semaphore
    sem_t *buffermutx = sem_open(BUFFERMUTX_SNAME, STARTUP_SFLAGS, STARTUP_SPERMS, 1);

    // wait for consumer process to begin
    printf("Waiting for consumer to start\n");
    sem_t *startsem1 = sem_open(STARTUP_SNAME1, STARTUP_SFLAGS, STARTUP_SPERMS, 0);
    sem_t *startsem2 = sem_open(STARTUP_SNAME2, STARTUP_SFLAGS, STARTUP_SPERMS, 0);
    sem_post(startsem1);
    sem_wait(startsem2);

    // start producing
    printf("Starting write:\n");

    int i = 30; // sup:remove
    while (i--) {

	printf(" sup: %d\n", __LINE__);
	sem_wait(bufferempt);
	printf("  sup: %d\n", __LINE__);
	sem_wait(buffermutx);

	printf("   writing at offset %d\n", i);

	sem_post(buffermutx);
	printf("    sup: %d\n", __LINE__);
	sem_post(bufferfull);
	printf("     sup: %d\n", __LINE__);
    }

    sleep(5);


    // cleanup semaphores
    sem_close(startsem1); sem_unlink(STARTUP_SNAME1);
    sem_close(startsem2); sem_unlink(STARTUP_SNAME2);
    sem_close(buffermutx); sem_unlink(BUFFERMUTX_SNAME);
    sem_close(bufferfull); sem_unlink(BUFFERFULL_SNAME);
    sem_close(bufferempt); sem_unlink(BUFFEREMPT_SNAME);

    // cleanup open files
    fclose(fp);

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

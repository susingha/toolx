#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "types_global.h"
#include "sem.h"
#include "sharedmem.h"
#include "shbuf.h"

char srcfile[] = "srcfile.txt";
extern int f;
extern int e;

char produce(FILE *fp) {
    return fgetc(fp);
}

int main() {
    FILE * fp = NULL;
    char c = 0;
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
    memset(s, 0, sizeof(shbuf_t));
    buf_init(s);

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

#define FILE
    while (TRUE) {

	c = produce(fp);

	sem_wait(bufferempt);
	sem_wait(buffermutx);

#ifdef FILE
	insert_item(s, c);
#endif

	sem_post(buffermutx);
	sem_post(bufferfull);

#ifdef FILE
	if (c == EOF)
	    break;
#endif
    }

    // cleanup open files
    fclose(fp);

    sleep(5);

    // cleanup semaphores
    sem_close(startsem1); sem_unlink(STARTUP_SNAME1);
    sem_close(startsem2); sem_unlink(STARTUP_SNAME2);
    sem_close(buffermutx); sem_unlink(BUFFERMUTX_SNAME);
    sem_close(bufferfull); sem_unlink(BUFFERFULL_SNAME);
    sem_close(bufferempt); sem_unlink(BUFFEREMPT_SNAME);

    printf("\nQueue full: %u times\n", f);
    return 0;
}



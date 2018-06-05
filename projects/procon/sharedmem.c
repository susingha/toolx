#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
 
char * get_sharedmem(key_t key, size_t size)
{
    int shmid;
    char * shm = NULL;

    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
 
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    return shm;
}


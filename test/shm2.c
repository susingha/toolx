#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>

#define SHMOBJ_PATH         "supshm"      
#define MAX_MSG_LENGTH      50
#define TYPES               8

struct msg_s {
    int type;
    char content[MAX_MSG_LENGTH];
};


int main(int argc, char *argv[]) {
    int shmfd;
    int shared_seg_size = (1 * sizeof(struct msg_s));
    struct msg_s *shared_msg;

    shmfd = shm_open(SHMOBJ_PATH, O_RDWR, S_IRWXU | S_IRWXG);
    if (shmfd < 0) {
	perror("In shm_open()");
	exit(1);
    }
    
    shared_msg = (struct msg_s *)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shared_msg == NULL) {
        perror("In mmap()");
        exit(1);
    }
    printf("Message type is %d, content is: %s\n", shared_msg->type, shared_msg->content);
    
    return 0;
}

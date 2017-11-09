#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>


#define SHMOBJ_PATH         "/data/local/tmp/supshm"
#define MAX_MSG_LENGTH      50
#define TYPES               8

struct msg_s {
    int type;
    char content[MAX_MSG_LENGTH];
};


int main(int argc, char *argv[]) {
    int fd;
    int shared_seg_size = (1 * sizeof(struct msg_s));
    struct msg_s *shared_msg;
    
    fd = open(SHMOBJ_PATH, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);
    ftruncate(fd, shared_seg_size);
    shared_msg = (struct msg_s *)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    srandom(time(NULL));
    shared_msg->type = 56; // random() % TYPES;
    snprintf(shared_msg->content, MAX_MSG_LENGTH, "sup: My message, type %d, num %ld", shared_msg->type, random());

    getchar();
   
#if 1
    if (unlink(SHMOBJ_PATH) != 0) {
        perror("In unlink()");
        exit(1);
    }
#endif

    return 0;
}

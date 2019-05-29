#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void fun2(int fd) {
    int i = 1;
    printf("sup: %s, local var at %p\n", __func__, &i);


    int rc;
    char buf[1024];
    rc = read(fd, (void*)buf, 1024);
    buf[rc] = '\0';
    printf("sup: %s\n", buf);
    
}

void fun1(int fd) {
    int i = 1;
    printf("sup: %s, local var at %p\n", __func__, &i);
    fun2(fd);
}

int main() {

    int fd = open("/dev/chardev", O_RDONLY);

    fun1(fd);

    close(fd);
    return 0;
    
}

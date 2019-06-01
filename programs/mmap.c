

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>

#include "template.h"

int main(int argc, char *argv[]) {
  printargs(argc, argv);


  int fd = open("test_file", O_RDWR | O_CREAT, (mode_t)0600);
  const char *text = "hello";
  size_t textsize = strlen(text) + 1;
  lseek(fd, textsize-1, SEEK_SET);
  write(fd, "", 1);
  char *map = mmap(0, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("sup: mmap at %p\n", map);
  memcpy(map, text, strlen(text));
  msync(map, textsize, MS_SYNC);
  munmap(map, textsize);
  close(fd);
  sleep(300);
  return 0;
}

#include <sys/types.h>
 
#define SHMKEY 5678
char * get_sharedmem(key_t key, size_t size);

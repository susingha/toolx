#include <sys/types.h>
 
#define SHKEY 5678
#define SHMSZ 27

char * get_sharedmem(key_t key, size_t size);

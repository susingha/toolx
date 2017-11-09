#ifndef READ4K
#define READ4K

#include <stdio.h>

#define BUF_4K 128 // 4098

unsigned int read4k(FILE * fp, void * buf4k_p);

#endif // ifndef READ4K

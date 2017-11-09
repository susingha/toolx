#include "read4k.h"


unsigned int read4k(FILE * fp, void * buf4k_p) {
    if(!fp)
	return 0;
    if(!buf4k_p)
	return 0;

    return fread(buf4k_p, 1, BUF_4K, fp);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if 0
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif


int main (int argc, char *argv[])
{
    int c;
    for (c = 0; c < argc; ++c) {
	printf("%s ", argv[c]);
    }
    printf("(%d)\n", argc);

    return 0;
}




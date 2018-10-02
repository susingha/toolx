

/*
 * Given a histogram. Find the volume of water it can hold
 *    
 *                                     +--+
 *                                     |24|
 *                                     |  |
 *          +--+  -  -  -  -  -  -  -  |  |
 *          |21|                       |  |
 *          |  |                       |  |
 *          |  |           +--+        |  |
 *          |  |           |18|        |  |
 *          |  |        +--+  |        |  +--+
 *          |  |        |16|  |        |  |16|
 *          |  |        |  |  |        |  |  |  _  _  _   __
 *          |  |        |  |  |     +--+  |  |           |14|
 *          |  |        |  |  |     |13|  |  |           |  |
 *        __|  |  +--+  |  |  |     |  |  |  +--+        |  |
 *       |11|  |  |11|  |  |  |     |  |  |  |11|        |  |
 *       |  |  |  |  |  |  |  +--+  |  |  |  |  |        |  +--+
 *       |  |  |  |  |  |  |  |09|  |  |  |  |  |        |  |09|
 *     __|  |  |__|  |  |  |  |  +--+  |  |  |  +--+     |  |  |
 *    |07|  |  |07|  |  |  |  |  |07|  |  |  |  |07|     |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |     |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |   __|  |  +--+
 *    |  |  |  |  |  +--+  |  |  |  |  |  |  |  |  |  |04|  |  |04|
 *    |  |  |  |  |  |03|  |  |  |  |  |  |  |  |  |__|  |  |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |02|  |  |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     07 11 21 07 11 03 16 18 09 07 13 24 16 11 07 02 04 14 09 04
 *    
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if 0
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif

#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

int main (int argc, char *argv[])
{
    int c;
    for (c = 0; c < argc; ++c) {
	printf("%s ", argv[c]);
    }
    printf("(%d)\n", argc);

    return 0;
}




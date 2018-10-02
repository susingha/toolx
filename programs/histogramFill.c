

/*
 * Given a histogram. Find the volume of water it can hold
 *    
 *                                      __ 
 *                                     |24|
 *              14 10 18 05 03 12 14 08|  |
 *           __   _  _  _  _  _  _  _  |  |
 *          |21|                       |  |
 *          |  |                       |  |
 *          |  |            __         |  |
 *          |  |           |18|        |  |
 *          |  |         __|  |        |  |__ 03 07 12 10
 *          |  |        |16|  |        |  |16|
 *          |  |        |  |  |        |  |  |  _  _  _   __
 *          |  |        |  |  |      __|  |  |           |14|
 *          |  |        |  |  |     |13|  |  |           |  |
 *        __|  |   __   |  |  |     |  |  |  |__         |  |
 *       |11|  |  |11|  |  |  |     |  |  |  |11|        |  |
 *       |  |  |  |  |  |  |  |__   |  |  |  |  |        |  |__ 
 *       |  |  |  |  |  |  |  |09|  |  |  |  |  |        |  |09|
 *     __|  |  |__|  |  |  |  |  |__|  |  |  |  |__      |  |  |
 *    |07|  |  |07|  |  |  |  |  |07|  |  |  |  |07|     |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |     |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |   __|  |  |__ 
 *    |  |  |  |  |  |__|  |  |  |  |  |  |  |  |  |  |04|  |  |04|
 *    |  |  |  |  |  |03|  |  |  |  |  |  |  |  |  |__|  |  |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |02|  |  |  |  |
 *    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     07 11 21 07 11 03 16 18 09 07 13 24 16 11 07 02 04 14 09 04
 *
 *    Total: 116
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

int maxof(int a, int b) {
    return a > b?a:b;
}

int minof(int a, int b) {
    return a < b?a:b;
}


int main (int argc, char *argv[])
{
    int num, i, max, min, col, total = 0;
    int bars[] = {7, 11, 21,  7, 11, 3, 16, 18, 9, 7,
	         13, 24, 16, 11,  7, 2,  4, 14, 9, 4};

    num = sizeof(bars)/sizeof(bars[0]);

    int *lscan = (int *) malloc(num * sizeof(bars[0]));
    int *rscan = (int *) malloc(num * sizeof(bars[0]));

    max = 0;
    for (i = 0; i < num; ++i) {
	max = maxof(max, bars[i]);
	lscan[i] = max;
    }
    
    
    max = 0;
    for (i = 0; i < num; ++i) {
	max = maxof(max, bars[num-i-1]);
	rscan[num-i-1] = max;
    }


    for (i = 0; i < num; ++i) {
	min = minof(lscan[i], rscan[i]);
	col = 0;
	if (bars[i] < min) {
	    col = min - bars[i];
	    total += col;
	}
	printf("water column over bar %3d: %3d\n", bars[i], col);
    }
    
    printf("                     -------------\n");
    printf("total volume:              %d\n", total);

    return 0;
}




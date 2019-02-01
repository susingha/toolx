
/* Use Monte Carlo method to calculate Pie */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include "template.h"

#if 0
#include <string.h>
#include <sys/types.h>
#endif

extern int randmax;

double getPie(double insidecircle, double outsidecircle)
{
    double ratio = insidecircle/outsidecircle;
    printf("insidecircle = %f, outsidecircle = %f, ratio = %f\n", insidecircle, outsidecircle, ratio);
    return (4*ratio)/(1 + ratio);
}

int main (int argc, char *argv[])
{
    printargs(argc, argv);

    randmax = 4000;
    unsigned int intarval = 0;
    unsigned int x, y, r = randmax;


    double insidecircle = 0;
    double outsidecircle = 0;

    while (TRUE) {

	x = getrandom();
	y = getrandom();

	if ((x*x + y*y) > (r*r)) {
	    outsidecircle++;
	} else {
	    insidecircle++;
	}

	if (!(++intarval) /* % 100000000 == 0 */)
	    printf("pie: %f\n", getPie(insidecircle, outsidecircle));

    }

    return 0;
}




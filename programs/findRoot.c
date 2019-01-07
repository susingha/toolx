
// Find the nth root of a number with an error range of 0.001

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "template.h"

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif

double root(int pwr, double num)
{
    double left  = 1;
    double right = num;
    double ans = num;

    while (fabs(pow(ans, pwr) - num) > 0.001) {

	if (pow(ans, pwr) > num) {
	    right = ans;
	} else {
	    left = ans;
	}
	ans = (left + right)/2;
    }

    return ans;
}

int main (int argc, char *argv[])
{
    printargs(argc, argv);

    int pwr = atoi(argv[1]);
    int num = atoi(argv[2]);

    trackon();
    printf("%dth root of %d = %f\n", pwr, num, root(pwr, num));
    trackoff(); trackprint();

    return 0;
}




/*
 * README: We require a sample input file for this program:
 * SampleInput.txt
 * ===============
 * Move 2
 * Turn right
 * Move 4
 * Turn left
 * Move -5
 * Turn right
 * Move 10
 * Turn left
 * Move -2
 * Turn left
 * Turn left
 * Move 5
 * Move -2
 * Turn right
 * Move 1
 * Move 0
 *
 */

#include <stdio.h>

int main() {

    char line[20];

    signed int posx = 0;
    signed int posy = 0;

    signed int xincr = 0;
    signed int yincr = 1;
    signed int txincr = 0;
    signed int tyincr = 0;

    signed int steps = 0;
    char dummy[20];

    FILE * fd = fopen("SampleInput.txt", "r");

    while(!feof(fd)) {
	fgets(line, sizeof(line), fd);
	printf("%s", line);


	if(line[0] = 'M') {
	    sscanf(line, "%s %d", dummy, &steps);
	    posx += xincr * steps;
	    posy += yincr * steps;
	} else if (line[0] == 'T') {

	    txincr = xincr;
	    tyincr = yincr;
	    if(line[5] == 'r') {
		if(xincr == 0)
		    xincr = 1;
		if(yincr == 0)
		    yincr = -1;


	    } else if(line[5] == 'l') {
		if(xincr == 0)
		    xincr = -1;
		if(yincr == 0)
		    yincr = 1;

	    }

	    if(txincr)
		xincr = 0;
	    if(tyincr)
		yincr = 0;
	}



    }

    printf("%d,%d", posx, posy);
    return 0;
}



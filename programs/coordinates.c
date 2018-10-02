/*
 * README: Start from coordinates 0,0 and follow the directions
 *         as given in input.txt and print the final coordinates
 *
 * input.txt
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
 * Answer should be 13, -8
 *
 */

#include <stdio.h>

int main() {

    char line[20];

    signed int posx = 0;
    signed int posy = 0;

    signed int xincr = 0; // initially facing north
    signed int yincr = 1;
    signed int txincr = 0;
    signed int tyincr = 0;

    signed int steps = 0;
    char dummy[20];

    FILE * fd = fopen("coordinates_input.txt", "r");

    while(!feof(fd)) {
	fgets(line, sizeof(line), fd);
	printf("%s", line);


	if(line[0] == 'M') {
	    sscanf(line, "%s %d", dummy, &steps);
	    posx += xincr * steps;
	    posy += yincr * steps;

	} else if (line[0] == 'T') {

	    txincr = xincr;
	    tyincr = yincr;

	    if (line[5] == 'r') { // right turn

		if (yincr) {
		    xincr = yincr;
		    yincr = 0;
		} else
		if (xincr) {
		    yincr = 0 - xincr;
		    xincr = 0;
		}
	    }

	    if (line[5] == 'l') { // left turn
		if (yincr) {
		    xincr = 0 - yincr;
		    yincr = 0;
		} else
		if (xincr) {
		    yincr = xincr;
		    xincr = 0;
		}
	    }
	}
    }

    printf("%d,%d\n", posx, posy);
    return 0;
}



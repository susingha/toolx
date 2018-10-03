
/*
 * Rotate a matrix by 90 degrees in place
 * ofcourse the matrix would have to be square
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define rnd() (rand()%100)
#define ARRMAX 4 // 23
#define ZERO   0
#define LIMT   (ARRMAX - 1)
#define FOUR   4
#define ONE    1
int arr[ARRMAX][ARRMAX];
int debug = 0;

void initMatrix() {
    for(int i = 0; i < ARRMAX; ++i)
	for(int j = 0; j < ARRMAX; ++j)
	    arr[i][j] = rnd();
}

void printMatrix() {
    for(int i = 0; i < ARRMAX; ++i) {
	for(int j = 0; j < ARRMAX; ++j)
	    printf("%3d,", arr[i][j]);
	printf("\n");
    }
    printf("\n");
}


/*
 * Method 1. Sweep each quardrant along the circumference
 *           Like a clock arm. More efficient than method 2
 */

void sweepcircle(int row, int col) {
    int t1, t2, t3, t4;

    t1 = arr[row][col];
    t2 = arr[col][ARRMAX-1-row];
    t3 = arr[ARRMAX-1-row][ARRMAX-1-col];
    t4 = arr[ARRMAX-1-col][row];

    arr[row][col] = t4;
    arr[col][ARRMAX-1-row] = t1;
    arr[ARRMAX-1-row][ARRMAX-1-col] = t2;
    arr[ARRMAX-1-col][row] = t3;
}

void rotateclockarm() {
    int i, j;

    /* only pick elements from the quardrant, right of a half diagonal */

    for (i = 0; i < ARRMAX-1; ++i) {
	for (j = i; j < ARRMAX-1-i; ++j) {
	    sweepcircle(i, j);
	}
    }
}


/*
 * Method 2. By spinning the matrix on a diagnonal (transpose)
 *           then spinning the matrix on a vertical axis
 *           simpler to understand but costlier than Method 1
 */

void transwap(int row, int col) {
    int tmp = arr[row][col];
    arr[row][col] = arr[col][row];
    arr[col][row] = tmp;

}

void horswap(int row, int col) {
    int tmp = arr[row][col];
    arr[row][col] = arr[row][ARRMAX-1-col];
    arr[row][ARRMAX-1-col] = tmp;
}

void spindiagaxis() { // transpose
    int i, j;
    for (i = 0; i < ARRMAX; ++i) {
	for (j = i; j < ARRMAX; ++j ) {
	    transwap(i, j);
	}
    }
}

void spinvertaxis() {
    int i, j;
    for (i = 0; i < ARRMAX; ++i) {
	for (j = 0; j < ARRMAX/2; ++j ) {
	    horswap(i, j);
	}
    }
}

void rotatespinaxis() {
    spindiagaxis();
    if (debug) printMatrix();

    spinvertaxis();
    if (debug) printMatrix();

}

int main() {
    initMatrix();
    printMatrix();

    printf("Using Method 1:\n");
    rotateclockarm();
    printMatrix();

    printf("Using Method 2:\n");
    rotatespinaxis();
    printMatrix();

    return 0;
}


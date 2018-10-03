
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
 * Method 1. Sweep each element along the circumference
 *           Like a clock arm. More efficient than method 2
 */

void rotate90 (int x, int y, int *i, int *j) {
    int oi = x;
    int oj = y;
    int ni = ZERO + oj;
    int nj = LIMT - oi;
    *i = ni;
    *j = nj;
}


void get_final(int rotations, int row1, int col1, int *row2p, int *col2p) {
    for (int i = 0; i < rotations; ++i) {
	rotate90 (row1, col1, row2p, col2p);
	row1 = *row2p;
	col1 = *col2p;
    }
}

void rotateclockarm() {

    int save = 0, move = 0;
    int i = 0, j = 0, k = 0;
    int begin = 0, end = 0;
    int row1 = 0, col1 = 0, row2 = 0, col2 = 0;

    for (i = 0; i < (ARRMAX)/2; ++i) {
	begin = ZERO + i;
	end   = LIMT - i;
	if(debug) printf("servicing: ");
	for (j = begin ; j < end; ++j) {
	    row1 = begin;
	    col1 = j;
	    if(debug) printf("%d:[%d][%d], ", arr[row1][col1], row1, col1);
	    save = arr[row1][col1];
	    for (k = 0; k < FOUR; ++k) {
		move = save;
		get_final(ONE, row1, col1, &row2, &col2);
		save = arr[row2][col2];
		arr[row2][col2] = move;
		row1 = row2;
		col1 = col2;
	    }
	}
	if(debug) printf("\n");
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

    printf("Using Method 1\n");
    rotateclockarm();
    printMatrix();

    printf("Using Method 2\n");
    rotatespinaxis();
    printMatrix();

    return 0;
}


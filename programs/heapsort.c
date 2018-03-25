
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARR_SIZE (50)


void printarray(int * array) {
    int i;
    for(i = 0; i < ARR_SIZE; ++i) {
	printf("%d, ", array[i]);
    }
    printf("\n");

    return;
}

int getleftchild(int rootindex) {
    return (2 * (rootindex + 1)) - 1;
}
int getrightchild(int rootindex) {
    return (2 * (rootindex + 1));
}
int getparent(int childindex) {
    return ((childindex + 1) / 2) - 1;
}

void swap(int * array, int index1, int index2) {
    int t = array[index1];
    array[index1] = array[index2];
    array[index2] = t;

    return;
}

void resolveheap(int * array, int size, int index) {
    int leftchildindex = getleftchild(index);
    int rightchildindex = getrightchild(index);

    if(leftchildindex < size) { // i have a left child
	resolveheap(array, size, leftchildindex);
// printf("\n array[%d] = %d, array[%d] = %d", index, array[index], leftchildindex, array[leftchildindex]);
	if(array[index] < array[leftchildindex]) {
	    swap(array, index, leftchildindex);
	}
    } // else return;

    if(rightchildindex < size) { // i have a right child
	resolveheap(array, size, rightchildindex);
// printf("\n array[%d] = %d, array[%d] = %d", index, array[index], rightchildindex, array[rightchildindex]);
	if(array[index] < array[rightchildindex]) {
	    swap(array, index, rightchildindex);
	}
    } // else return;

}






int main() {
    int array[ARR_SIZE]; // indices: 0 to 49
    int i;

    srand(time(NULL));

    for(i = 0; i < ARR_SIZE; ++i) {
	array[i] = rand()%100;
    }

    printarray(array);
    for(i = 0; i < ARR_SIZE; ++i) {
	resolveheap(array + i, ARR_SIZE - i, 0);
    }
    printarray(array);

    return 0;
}
















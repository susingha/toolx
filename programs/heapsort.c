
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARR_SIZE (10)


void printarray(int * array) {
    int i;
    int sorted = 1;
    int prev = -1;
    for(i = 0; i < ARR_SIZE; ++i) {
        printf("%d, ", array[i]);
        if (array[i] < prev) {
            sorted = 0;
        }
        prev = array[i];
    }
    if (sorted) {
        printf("\nok\n");
        return;
    } else {
        printf("\nNOT sorted\n");
        return;
    }

    return;
}

int getleftchild(int rootindex) {
    return (2 * rootindex) + 1;
}
int getrightchild(int rootindex) {
    return (2 * rootindex) + 2;
}
int getparent(int childindex) {
    return (childindex - 1)/2;
}

void swap(int * array, int index1, int index2)
{
    int t = array[index1];
    array[index1] = array[index2];
    array[index2] = t;

    return;
}

void maxheapify(int * array, int size, int i)
{
    int max = i;
    int leftchildindex = getleftchild(i);
    int rightchildindex = getrightchild(i);

    if (leftchildindex < size) { // have a left child
	if (array[leftchildindex] > array[max])
	    max = leftchildindex;
    }

    if (rightchildindex < size) { // have a right child
	if (array[rightchildindex] > array[max])
	    max = rightchildindex;
    }

    if (max != i) {
	swap(array, i, max);
	maxheapify(array, size, max);
    }
}

int main() {
    int array[ARR_SIZE]; // indices: 0 to 49
    int i;

    srand(0 /*time(NULL)*/);

    for(i = 0; i < ARR_SIZE; ++i)
	array[i] = rand()%100;

    printarray(array);

    for(i = ARR_SIZE-1; i >= 0; --i)
	maxheapify(array, ARR_SIZE, i);

    printarray(array);

    for(i = 0; i < ARR_SIZE; ++i) {
	maxheapify(array, ARR_SIZE-i, 0);
	swap(array, 0, ARR_SIZE-1-i);
    }

    printarray(array);

    return 0;
}
















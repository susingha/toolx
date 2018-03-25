#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define rnd() (rand()%100)
#define ARRMAX 50
int arr[ARRMAX];
int debug = 0;


void insertValues() {
    int i = 0, num;
    printf("sup: Insert Values: ");
    for(i = 0; i < ARRMAX; ++i) {
	num = rnd();
	printf("%d, ", num);
	arr[i] = num;
    }
    printf("\n");
}

void printValues() {
    int i = 0;
    printf("sup: Sorted Values: ");
    for(i = 0; i < ARRMAX; ++i) {
	printf("%d, ", arr[i]);
    }
    printf("\n");
}

void swapin(int low, int high) {
    int t;

    if (debug)
    printf("sup: [%d] <--> [%d]\n", low, high);

    t = arr[low];
    arr[low] = arr[high];
    arr[high] = t;
}

#define NOCROSSCOND(x, y) (x < y)

void quicksort(int begin, int end) {
    int pivot = (begin + end) / 2;
    int pivotnum = arr[pivot];
    int low = begin, high = end;
    int final = pivot;

    if (debug)
    printf("sup: partitioning %d to %d with pivotnum = %d at %d\n", begin, end, pivotnum, pivot);

    if(!NOCROSSCOND(begin, end))
	return;

    while (NOCROSSCOND(low, high)) {
	while (low <= end && arr[low] <= pivotnum)
	    low++;
	while (high >= begin && pivotnum <= arr[high])
	    high--;

	if(NOCROSSCOND(low, high))
	    swapin(low, high);
    }

    if (low < pivot)
	final = low;
    if (pivot < high)
	final = high;

    swapin(pivot, final);

    if(debug)
    printValues();

    quicksort(begin, final - 1);
    quicksort(final + 1, end);

    return;
}

int main() {
    insertValues();
    quicksort(0, ARRMAX - 1);
    printValues();

    return 0;
}

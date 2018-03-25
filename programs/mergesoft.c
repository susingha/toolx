#include <stdio.h>

#define ARR_SIZE (50)

void printarray(int * array) {
    int i;
    for(i = 0; i < ARR_SIZE; ++i) {
	printf("%d, ", array[i]);
    }
    printf("\n");

    return;
}


void merge(int res[], int low1, int high1, int low2, int high2, int array[]) {
    int i = 0;

    while (low1 <= high1 && low2 <= high2) {
        if (array[low1] <= array[low2]) {
            res[i] = array[low1];
            low1++;
            i++;
        } else {
            res[i] = array[low2];
            low2++;
            i++;
        }
    }

    while (low1 <= high1) {
        res[i] = array[low1];
        low1++;
        i++;
    }

    while (low2 <= high2) {
        res[i] = array[low2];
        low2++;
        i++;
    }
}
    

void mergesort (int * A, int low, int high, int * B) {
    int div;

    if (low > high)
        return;
    if (low = high) {
        B[low] = A[low];
        return;
    }

    div = (low + high)/2;
    mergesort(B, low, div, A);
    mergesort(B, div+1, high, A);

    merge(A, low, div, div+1, high, B);
}


#if 0
int main() {
    int i;
    int arr[10] = {0,1,2,3,4,5,6,7,8,9};
    int res[10];
    merge(res, 0, 4, 5, 9, arr);
    for (i = 0; i < 10; i++) {
            printf("%d, ", res[i]);
    }
    printf("\n");
}

#else

int main() {
    int array[ARR_SIZE]; // indices: 0 to 49
    int arraz[ARR_SIZE]; // indices: 0 to 49
    int i;

    srand(time(NULL));

    for(i = 0; i < ARR_SIZE; ++i) {
	array[i] = rand()%100;
    }

    mergesort(array, 0, ARR_SIZE - 1, arraz);
    printarray(array);

}
#endif

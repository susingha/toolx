#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "template.h"

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif


struct node {
    int num;
    char c;
    struct node *next, *prev; // used for linkedlist
    struct node *lptr, *rptr; // used for tree
};

void getarr(int arr[], int n)
{
    while (n--) arr[n] = getrandom();
}

void showarr(int arr[], int n)
{
    int i = 0;
    while (n--) printf("%d, ", arr[i++]);
    printf("\n");
}


int arr[1024];
int len = 10;

int main (int argc, char *argv[])
{
    printargs(argc, argv);

    trackon();

    getarr(arr, len);
    showarr(arr, len);
    loadmem();
    loadcpu(0);

    trackoff(); trackprint();

    return 0;
}




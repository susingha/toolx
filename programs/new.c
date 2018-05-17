#include <stdio.h>

int i;

int main () {
    unsigned int max = sizeof(void *);
    printf("%u\n", max);
    return 0;
}

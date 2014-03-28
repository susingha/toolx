#include <stdio.h>
#include <stdlib.h>

struct mov {
    int a;
    char p1[0];   // doesnt count (no padding)
    int b;
    char p2[3];   // counts as 4 bytes (1 byte padding)
    int c;
    short int sd; // counts as 4 bytes (2 byte padding)
    int d;
};

struct new {
};

union uu {
    int a;
    char p1;
};

int main() {
    struct mov m;
    m.a = 56;
    int arr[] = {1,2,3,4,5};

    struct mov * mp = NULL;
    struct mov * mq = mp + 1;
    printf("%u\n", mp);
    printf("%u\n", mq);
    printf("size = %ld\n", ((unsigned long int)mq - (unsigned long int)mp));

    printf("unsigned long = %d, struct = %d, union = %d, array = %d, new = %d\n", (int) sizeof(unsigned long),
	                                                              (int) sizeof(m),
							              (int) sizeof(union uu),
							              (int) sizeof(arr),
								      (int) sizeof(struct new));




    return 0;
}




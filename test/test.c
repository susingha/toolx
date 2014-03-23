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

union uu {
    int a;
    char p1;
};

int main() {
    struct mov m;
    m.a = 56;
    int arr[] = {1,2,3,4,5};
    int i = 4, j = 3;

    char * str1 = "ABC";
    char str2[] = "DEF";

    struct mov * mp = NULL;
    struct mov * mq = mp + 1;
    printf("%u\n", mp);
    printf("%u\n", mq);
    printf("size = %ld\n", ((unsigned long int)mq - (unsigned long int)mp));

    printf("unsigned long = %d, struct = %d, union = %d, array = %d, long long int = %d\n", (int) sizeof(unsigned long),
	                                                              (int) sizeof(m),
							              (int) sizeof(union uu),
							              (int) sizeof(arr),
								      (int) sizeof(long long int));

    printf("%s \n%s \n", str1, str2);
    *str1 = 'X';
    *str2 = 'Y';
    printf("%s \n%s \n", str1, str2);


    printf("%f", (float)i/(float)j);


    printf("\n\n");
    return 0;
}




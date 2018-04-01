#include <stdio.h>
#include <string.h>

int main() {
    char str[100];
    int len = 0;
    int count;
#if 0
    strcpy(str, "eowr");
#else
    printf("Input your string: ");
    scanf("%s", str);
#endif
    len = strlen(str);
    printf("string: %s length = %d\n\n", str, len);

    count = rotate(str, len, str, 0);

    printf("length = %d, total = %d\n", len, count);

    return 0;
}


rotate(char * A, int len, char * str, int count) {
    char c;
    int i, j;

    if (len == 1) {
        printf("%3d. %s\n", ++count, str);
        return count;
    }
    
    for (j = 0; j < len; ++j) {

        // Rotate once
        c = A[0];
        for (i = 0; i < len-1; ++i) {
            A[i] = A[i+1];
        }
        A[i] = c;

        count = rotate(A + 1, len - 1, str, count);
    }
    return count;
}




















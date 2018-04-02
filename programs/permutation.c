#include <stdio.h>
#include <string.h>

int perm(char * A, int len, char * str, int count);

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

    count = perm(str, len, str, 0);

    printf("length = %d, total = %d\n", len, count);

    return 0;
}



void rotate(char * str, int len)
{
    char c;
    int i;
    // Rotate once
    c = str[0];
    for (i = 0; i < len-1; ++i) {
	str[i] = str[i+1];
    }
    str[i] = c;
}


int perm(char * A, int len, char * str, int count)
{
    int j;

    if (len == 1) {
        printf("%3d. %s\n", ++count, str);
        return count;
    }
    
    for (j = 0; j < len; ++j) {
	rotate(A, len);
        count = perm(A + 1, len - 1, str, count);
    }
    return count;
}




















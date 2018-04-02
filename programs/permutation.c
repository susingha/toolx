#include <stdio.h>
#include <string.h>

int perm_rec(char * A, int len, char * str, int count);
int perm_itr(char * A, int len);

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

    printf("recursive:\n");
    count = perm_rec(str, len, str, 0);
    printf("length = %d, total = %d\n\n", len, count);

    printf("iterative:\n");
    count = perm_itr(str, len);
    printf("length = %d, total = %d\n", len, count);

    return 0;
}


/* Recursive */

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

int perm_rec(char * A, int len, char * str, int count)
{
    int j;

    if (len == 1) {
        printf("%3d. %s\n", ++count, str);
        return count;
    }
    
    for (j = 0; j < len; ++j) {
        count = perm_rec(A + 1, len - 1, str, count);
	rotate(A, len);
    }
    return count;
}


void assert(int i, int j) {
    if (i != j) {
	printf("assert fail: %d != %d\n", i, j);
    }
}

#define TRUE 1
#define FALSE 0

/* Iterative */

int perm_itr(char * A, int len)
{
    int done = FALSE;
    int curr = len;
    int i, j = 0;

    int rcount[len+1];

    for (i = 0; i < len + 1; i++) {
	rcount[i] = 0;
    }

    while (!done) {
	if (curr > 1) {
	    if (rcount[curr] < curr) {
		rcount[curr]++;
		rotate(A + (len - curr), curr);
		curr--;
		rcount[curr] = 0;
	    } else {
		assert(rcount[curr], curr);
		if (curr == len)
		    done = TRUE;
		else
		    curr = curr + 1;

	    }
	} else {
	    assert(curr, 1);
	    j++;
	    printf("%3d. %s\n", j, A);
	    curr = curr + 1;
	}
    }

    return j;

}
















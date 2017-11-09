#include <stdio.h>
#include <string.h>

int count;
int main() {
    char str[100];
    int len = 0;
 // strcpy(str, "eowr");
    printf("Input your string: ");
    scanf("%s", str);
    len = strlen(str);
    printf("string: %s length = %d\n\n", str, len);

    rotate(str, len - 1);

    printf("length = %d, total = %d\n", len, count);

    return 0;
}

int rotate(char * substr, int index) {

    int i = 0, j = 0;
    char last;
    for(j = 0; j <= index; ++j) {

	// rotate the string
	last = substr[index];
	for(i = index; i > 0; --i)
	    substr[i] = substr[i-1];
	substr[0] = last;
	if(index == 1) {
	    printf("permutation = %s\n", substr);
	    ++count;
	}
	rotate(substr, index - 1);
    }

    return 0;
}

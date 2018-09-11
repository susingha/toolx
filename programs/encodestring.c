#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/* Print the character and the number of times it appeared */

char *str[] = {"aaaaabbbccccddeefgh",
               "abcddddeeeffghijjjk",
               "abcefghijlkmnopqrst",
               "abc",
	       "aaa",
	       "abb",
	       "aab",
               "a",
               ""};


int main () {
    int i = 0;
    int row = 0, count = 1;

    for(int j = 0; j < 9; j++) {
	row = j;
	printf("%s\n", str[row]);
	i = 0;
	count = 1;

	while(str[row][i] != '\0') {

	    if(str[row][i] != str[row][i+1]) {
		printf("%c%d", str[row][i], count);
		count = 1;
	    } else {
		count++;
	    }
	    i++;
	}

	printf("\n\n");
    }
    return 0;
}

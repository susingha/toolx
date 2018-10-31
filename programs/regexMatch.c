#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Not a regex match program but more of a wild card match program with * and ?
// Does not compile. Needs some functions to be implemented

#define TRUE 1
#define FALSE 0

int ischar(char c) {
    if ('a' <= c && c <= 'z')
	return TRUE;
    if ('A' <= c && c <= 'Z')
	return TRUE;
    if ('0' <= c && c <= '9')
	return TRUE;
    if (c == '\0')
	return TRUE;
    if (c == '?')
	return TRUE;
    if (c == '*')
	return TRUE;

    return FALSE;
}

int findNext(char c, char str[]) {
    int i;
    for (i = 0; str[i] != c && str[i] != '\0'; ++i);

    if (str[i] == c) {
	printf("sup: found %c at index %d\n", c, i);
	return i;
    }

    return -1;
}

int same (char regex[], char string[], int i) {

    if (!ischar(string[i]))
	return FALSE;

    if (!ischar(regex[i]))
	return FALSE;

    if (regex[i] == string[i] ||
	regex[i] == '?')
	return TRUE;
    return FALSE;
}
int compare(char regex[], char string[])
{
    int i, j, nextc, match;
    printf("compare: %s and %s\n", regex, string);

    for (i = 0; same(regex, string, i); ++i) {
	if (string[i] == '\0') return TRUE;
    }

    if (regex[i] == '\0')
	return FALSE;
    if (string[i] == '\0')
	return FALSE;

    nextc = -1;
    match = FALSE;

    if (regex[i] == '*') {
	j = i;

	do {
	    nextc = findNext(regex[i+1], &string[j]); // returns -1 if not found

	    if (nextc >= 0) {
		match = compare(&regex[i+1], &string[nextc]);
		j = nextc + 1;
	    }

	} while (nextc >= 0 && match == FALSE);

	    
    } else {
	return FALSE;
    }

    return match;
}

void showmatch(char regex[], char string[]) {

    if (compare(regex, string)) {
	printf("regex: %20s : %20s => Match\n", regex, string);
    } else {
	printf("regex: %20s : %20s => NO match\n", regex, string);
    }
}

int main()
{
    showmatch("*cat", "this catscat");

    return 0;
}

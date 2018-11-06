#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>

/*
 * Not a wildcard match program matching usual meanings of * and ?
 * find a better algorithm
 */

#define TRUE 1
#define FALSE 0
int debug = FALSE;

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

int findNext(char c, char str[], int start_offset) {
    int i;

    if (debug) printf("sup: finding %c in %s starting from offset %d\n", c, str, start_offset);

    for (i = start_offset; str[i] != c && str[i] != '\0'; ++i);

    if (str[i] == c) {
	if (debug) printf("sup: found next %c at index %d\n", c, i);
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
    int i, nextc, match, search_offset;

    if (debug) sleep(1);
    if (debug) printf("compare: '%s' and '%s'\n", regex, string);

    for (i = 0; same(regex, string, i); ++i) {
	if (debug) printf("sup: %c at index %d is match\n", string[i], i);
	if (string[i] == '\0') return TRUE;
    }

    if (regex[i] != '*')
	return FALSE;
    else if (regex[i] == '\0')
	return FALSE;

    nextc = -1;
    match = FALSE;
    search_offset = 0;

    while (match == FALSE) {
	nextc = findNext(regex[i+1], &string[i], search_offset); // returns -1 if not found

	if (nextc == -1)
	    return FALSE;

	match = compare(&regex[i+1], &string[i + nextc]);
	search_offset = nextc + 1;
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
    showmatch("*cat", "cat");
    showmatch("*cat*", "cat");
    showmatch("cat*", "cat");
    showmatch("*cat", "this cat");
    showmatch("*cat", "this catscat");
    showmatch("*cat", "this cacatscat");
    showmatch("this*cat", "this cacatscat");
    showmatch("this*cat*", "this cacatscat");
    showmatch("th*ca*cat", "this cacatscat");

    return 0;
}

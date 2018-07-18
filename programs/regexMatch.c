#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Not a regex match program but more of a wild card match program with * and ?

#define TRUE 1
#define FALSE 0

int ischar(char c) {
    if ('a' <= c && c <= 'z')
	return TRUE;
    if ('A' <= c && c <= 'Z')
	return TRUE;
    if ('0' <= c && c <= '9')
	return TRUE;
    return FALSE;
}

int charmatch(char c_regex, char c_string) {

    if (ischar(c_regex) &&
	ischar(c_string) && c_regex == c_string)
	return TRUE;

    if (c_regex == '?' && ischar(c_string))
	return TRUE;

    return FALSE;
}

int compare(char regex[], char string[])
{
    int lenr = strlen(regex);
    int lens = strlen(string);
    int match = FALSE;
    i = 0;

    char r = regex[0];
    cahr s = string[0];

    // base case
    if (r == '\0' && s == '\0')
	return TRUE;
    if (r == '\0')
	return FALSE;
    if (s == '\0')
	return FALSE;

    if (r == '*') {
	ri = findNextr();
	r = regex[ri];

	while(!match) {

	    ci = findNextMatch(string, r);
	    c = string[ci];

	    match = compare();
	}
    }

    if (charmatch(r, s)) {
	return compare(&regex[1], &string[1]);
    }

    return FALSE;
}


void main()
{
    char regex[1024];
    char string[1024];

    strcpy(regex, "*cat"); strcpy(string, "this cat"):

    if (compare(regex, string)) {
	printf("regex: %20s : %20s => Match\n");
    } else {
	printf("regex: %20s : %20s => NO match\n");
    }
}

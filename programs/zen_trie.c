
/*
 * Dictionary using trie.
 * pending to implement:
 * lookup recursive
 * delete recursive
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0
#define MAXWORDLEN 1024


struct node {
    char c;
    char end;
    struct node * child[26];
};

struct node * newnode(char c)
{
    struct node * new = (struct node *) calloc(1, sizeof(struct node));
    new->c = c;
    return new;
}

int ascix(char c)
{
    return (c - 'a');
}

int lookup_rec(struct node * heads[], char arr[]) {
    return 0;
}

int lookup_itr(struct node * heads[], char arr[])
{
    struct node * this = NULL;
    int i = 0;
    int len = strlen(arr);
    char c;
    
    for (i = 0; i < len; ++i) {
	c = arr[i];
	this = heads[ascix(c)];

	if (this && this->c == c) {
	    // c is present
	    if (i+1 == len && this->end == FALSE) // last char
		return FALSE;

	    heads = this->child;
	} else {
	    return FALSE;
	}
    }

    return TRUE;;
}

int print_rec(struct node * heads[], char arr[], int i)
{
    int j, num = 0;
    struct node * this = NULL;

    if (!arr) {
	arr = calloc(sizeof (char), MAXWORDLEN);
    }

    for (j = 0; j < 26; ++j) {
	this = heads[j];
	if (this) {

	    arr[i] = this->c;
	    if (this->end) {
		arr[i+1] = '\0';
		printf("%s\n", arr);
		num++;
	    }
	    num += print_rec(this->child, arr, i+1);
	}
    }

    return num;
}

void insert_rec(struct node * heads[], char word[])
{
    struct node * new = NULL;
    struct node * this = NULL;
    char c = word[0];
    char n = word[1];

    if (c == '\0') {
	return;
    }

    if (heads[ascix(c)] == NULL)
	heads[ascix(c)] = newnode(c);

    this = heads[ascix(c)];
    this->c = c;      // Not needed, done in newnode()
    if (n == '\0') {  // last char
	this->end = TRUE;
	return;
    }

    insert_rec(this->child, &word[1]);
}

void insert_itr(struct node * heads[], char word[])
{
    int i;
    int len = strlen(word);
    struct node * this = NULL;
    char c;

    for (i = 0; i < len; ++i) {
	c = word[i]; // printf("%c", c);

	if (!('a' <= c && c <= 'z')) {
	    printf("Cannot insert %c\n", c);
	    printf("Ensure words are:\n"
		   "between a to z in lower case\n"
		   "lower case\n"
		   "no spaces\n"
		   "no numbers\n"
		   "no capitals\n");
	    exit(1);
	}

	if (heads[ascix(c)] == NULL)
	    heads[ascix(c)] = newnode(c);

	this = heads[ascix(c)];
	this->c = c;      // Not needed, done in newnode()
	if (i + 1 == len) // last char
	    this->end = TRUE;

	heads = this->child;
    }
}

int main () {
    char word[MAXWORDLEN];
    char done = 0;
    int num, i;

    struct node * heads[26];
    for (i = 0; i < 26; ++i)
	heads[i] = NULL;

    done = 0;
    while (!done) {
	num++;
	scanf("%s", word);
	printf("%3d. Adding: %s\n", num, word);
#if 0
	insert_rec(heads, word);
#else
	insert_itr(heads, word);
#endif

	if (strcmp(word, "ok") == 0)
	    done = 1;
    }
    printf("Added %d words to Dictionary\n", num);

    num = print_rec(heads, NULL, 0);
    printf("Found %d words in Dictionary\n", num);

    done = 0;
    while (!done) {
	printf("Enter word to lookup: ");
	scanf("%s", word);
	printf("Finding: %10s", word);
	if (lookup_itr(heads, word)) {
	    printf(" - found\n");
	} else {
	    printf(" - NOT found\n");
	}

	if (strcmp(word, "ok") == 0)
	    done = 1;
    }
}



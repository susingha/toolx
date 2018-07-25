
/*
 * Dictionary using trie.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "zen_types.h"
#include "zen_ascii.h"
#include "zen_sender.h"

#define MAXWORDLEN 1024
#define MAXWORDLENSAFE 128

struct node {
    char c;
    char end;
    struct node ** child;
};

static struct node ** trie_heads_main = NULL;

int ascii_max = 0;

struct node * newnode(char c)
{
    struct node * new = (struct node *) calloc(1, sizeof(struct node));
    new->c = c;
    new->child = (struct node **) calloc(ascii_max, sizeof(struct node *));
    return new;
}

int trie_set_heads(struct node ** heads)
{
    trie_heads_main = heads;
    return 0;
}

struct node ** trie_get_heads()
{
    return trie_heads_main;
}

int print_rec(struct node * heads[], char arr[], int i, int forceprint)
{
    int j, num = 0, strlen = 0;
    struct node * this = NULL;

    if (i > MAXWORDLENSAFE)
	return num;

    if (!arr) {
	arr = calloc(sizeof (char), MAXWORDLEN);
    }

    for (j = 0; j < ascii_max; ++j) {
	this = heads[j];
	if (this) {

	    arr[i] = this->c;
	    if (this->end) {
		arr[i+1] = '\n';
		arr[i+2] = '\0';
		strlen = i + 2;
		if (forceprint) {
		    if (debug || runverbose || debugnetwork)
			printf("%s\n", arr);
		    zen_sock_write(arr, strlen);
		}
		num++;
	    }
	    num += print_rec(this->child, arr, i+1, forceprint);
	}
    }

    return num;
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
    
    char matches[MAXWORDLEN];
    uint matchcount;

    for (i = 0; i < len; ++i) {
	c = arr[i];
	matches[i] = c;
	this = heads[ascix(c)];

	if (this && this->c == c) {                     // c is present

	    if (i+1 == len) {                           // last char

		matchcount = print_rec(this->child, matches, i+1, 1);
		if (debug)
		    printf("Found %u matches\n", matchcount);

		if (this->end == FALSE)
		    return FALSE;
	    }

	    heads = this->child;
	} else {
	    return FALSE;
	}
    }


    return TRUE;;
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

#if 0
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
#endif

	if (heads[ascix(c)] == NULL)
	    heads[ascix(c)] = newnode(c);

	this = heads[ascix(c)];
	this->c = c;      // Not needed, done in newnode()
	if (i + 1 == len) // last char
	    this->end = TRUE;

	heads = this->child;
    }
}


char filename[] = "zen_domains.txt";
char word[MAXWORDLEN];


int start_trie () {
    size_t maxlen = MAXWORDLEN;
    char done = 0;
    int num, i;
    struct node ** heads = NULL;

    FILE * ff = fopen(filename, "r");
    if (!ff)
	return -1;

    ascii_max = ascix_setup();
    if (!ascii_max)
	return -1;

    heads = calloc(ascii_max, sizeof(struct node *));
    trie_set_heads(heads);

    while (fscanf(ff, "%s", word) != EOF) {
	num++;
	if (runverbose)
	    printf("%3d. Adding: %s\n", num, word);

#if 0
	insert_rec(heads, word);
#else
	insert_itr(heads, word);
#endif

	if (runslow) {
	    printf("sleep 1 before read next domain\n");
	    sleep(1);
	}
    }

    printf("Added %d words to Dictionary\n", num);
    
    num = print_rec(heads, NULL, 0, 0);
    printf("Found %d words in Dictionary\n", num);

#if 0
    done = 0;

    while (!done) {
	printf("Enter word to lookup: ");
	scanf("%s", word);

#if 0
	if (strcmp(word, "ok") == 0)
	    return 0;
#endif

	if (lookup_itr(heads, word)) {
	    printf("%-10s - found\n\n", word);
	} else {
	    printf("%-10s - NOT found\n\n", word);
	}

    }
#endif
    return 0;
}

int zen_trie_init() {
    return start_trie();
}

char err_bad_query[] = "Bad Query";

int serve_results(char query[], int n, int sock)
{
    struct node ** heads = NULL;
    // we can assume that all n bytes in query is within buffer

    if (!sanitize(query, n)) {
	write(sock, err_bad_query, strlen(err_bad_query));
	printf("%s\n", err_bad_query);
	return -1;
    }

    heads = trie_get_heads();
    if (!heads)
	return 0;

    lookup_itr(heads, query);

    return 0;
}


int zen_write_results(char query[], int n, int sock)
{
    return serve_results(query, n, sock);
}

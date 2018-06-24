
// Implement a dictionary using trie.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0
#define MAXWORDLEN 1024


struct node {
    char c;
    char end;
    struct node *child[26];
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


int print_rec(struct node * this, char arr[], int i)
{
    int j, num = 0;

    if (!arr) {
	arr = calloc(sizeof (char), MAXWORDLEN);
	i = -1;
    }

    if (this->c != '\0')
	arr[i] = this->c;

    if (this->end == TRUE) {
	arr[i+1] = '\0';
	printf("%s\n", arr);
	num++;
    }

    for (j = 0; j < 26; ++j) {
	if (this->child[j]) {
	    num += print_rec(this->child[j], arr, i+1);
	}
    }

    return num;
}

void insert_itr(struct node * head, char word[])
{
    int i;
    int len = strlen(word);
    struct node * this = head;
    struct node * new = NULL;
    char c;

    for (i = 0; i < len; ++i) {
	c = word[i]; // printf("%c", c);

	if (this->child[ascix(c)] == NULL)
	    this->child[ascix(c)] = newnode(c);

	this = this->child[ascix(c)];

	this->c = c;
	if (i + 1 == len) // end char
	    this->end = TRUE;
    }
}

int main () {
    struct node * head = newnode('\0');
    char word[MAXWORDLEN];
    char done = 0;
    int num;

    while (!done) {
	num++;
	scanf("%s", word);
	printf("%3d. Adding: %s\n", num, word);
	insert_itr(head, word);

	if (strcmp(word, "ok") == 0)
	    done = 1;
    }
    printf("Added %d words to Dictionary\n", num);

    num = print_rec(head, NULL, 0);
    printf("Found %d words in Dictionary\n", num);
}



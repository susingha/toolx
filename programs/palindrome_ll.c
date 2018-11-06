#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Test if a string represented in the form of a linked list is a palindrome */

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif

#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

#define MEASURE_RUNTIME
#ifdef MEASURE_RUNTIME
#include <time.h>
clock_t ts;
#endif

#define RANDOM
#ifdef RANDOM
#define rnd() (rand()%100)
#else
#define rnd() (dataset[dataindex++])
#endif


struct node {
    char c;
    struct node * next;
};

struct node * createll(char * str)
{
    int i = 0;
    char c;
    struct node * new = NULL;
    struct node * head = NULL;

    while ((c = str[i++]) != ENDS) {

	if (head) {
	    new->next = malloc(sizeof(struct node));
	    new = new->next;
	} else {
	    new = malloc(sizeof(struct node));
	    head = new;
	}

	new->c = c;
	new->next = NULL;
    }

    return head;
}

void printll(struct node * head)
{
    while(head) {
	printf("%c", head->c);
	head = head->next;
    }
    printf("\n");
}

void freell(struct node * head)
{
    struct node * this = head;
    while(this) {
	head = this->next;
	free(this);
	this = head;
    }
}

int testPalindrome(struct node * head,
	           struct node * this,
		   int i)
{
    int i

    if (this->next)
	testPalindrome(this->next);
	



    return 0;
}

int main (int argc, char *argv[])
{
    int c;
    for (c = 0; c < argc; ++c) {
	printf("%s ", argv[c]);
    }
    printf("(%d)\n", argc);

    struct node * strll = NULL;
    
    strll = createll("abcdefgh");
    printll(head);
    testPalindrome(strll);
    freell(strll);

    return 0;
}




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Test if a string represented in the form of a linked list is a palindrome */

#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

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

/* curr pointer is the left  probe moving from left to right. this is global
 * this pointer is the right probe moving from right to left as the recursion returns
 */
struct node * curr = NULL;
int recurse(struct node * this)
{
    int ret = TRUE;

    if (!this || !curr)
	return FALSE;

    if (this->next)
	ret = recurse(this->next);

    if (ret == FALSE)
	return FALSE;

    if (this->c == curr->c) {
	curr = curr->next;
	return TRUE;
    }

    return FALSE;
}


void testPalindrome(char str[])
{
    struct node * strll = NULL;
    
    strll = createll(str);

    curr = strll;
    if (recurse(strll))
	printf("%s is a palindrome\n", str);
    else
	printf("%s is NOT a palindrome\n", str);

    freell(strll);
}

int main (int argc, char *argv[])
{
    int c;
    for (c = 0; c < argc; ++c) {
	printf("%s ", argv[c]);
    }
    printf("(%d)\n", argc);

    testPalindrome("");
    testPalindrome("a");
    testPalindrome("ab");
    testPalindrome("aa");
    testPalindrome("aba");
    testPalindrome("abba");
    testPalindrome("adam");
    testPalindrome("adad");
    testPalindrome("abacus");
    testPalindrome("abaaba");

    return 0;
}




/*
 * An XOR linked list is a more memory efficient doubly linked list.
 * Instead of each node holding next and prev fields,
 * it holds a field named both, which is an XOR of the next node and the previous node.
 * Implement an XOR linked list
 * it has an add(element) which adds the element to the end,
 * and a get(index) which returns the node at index.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "template.h"

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif


struct node {
    int num;
    struct node *both;
};

struct node * head = NULL;

void add(int num)
{
    struct node *this = head, *prev = NULL, *next = NULL;
    struct node *new = (struct node *)malloc(sizeof(struct node));
    new->num = num;
    new->both = NULL;

    while (this) {
	next = (struct node *)((uintptr_t)this->both ^ (uintptr_t)prev);
	prev = this;
	this = next;
    }

    if (prev)
	prev->both = (struct node *)((uintptr_t)prev->both ^ (uintptr_t)new);

    new->both = prev;

    if (!head)
	head = new;

    return;
}

int  get(struct node * head, unsigned int idx)
{
    int i = 0;
    struct node *this = head, *prev = NULL, *next = NULL;

    while (this) {

	if (i++ == idx)
	    return this->num;

	next = (struct node *)((uintptr_t)this->both ^ (uintptr_t)prev);
	prev = this;
	this = next;
    }

    return -1;
}

int main (int argc, char *argv[])
{

    int num = 0;

    num = getrandom();
    printf("inserting: %d\n", num);
    add(num);

    num = getrandom();
    printf("inserting: %d\n", num);
    add(num);

    num = getrandom();
    printf("inserting: %d\n", num);
    add(num);

    printf("List[%d] = %d\n", 0, get(head, 0));
    printf("List[%d] = %d\n", 1, get(head, 1));
    printf("List[%d] = %d\n", 2, get(head, 2));
    printf("List[%d] = %d\n", 3, get(head, 3));

    return 0;
}




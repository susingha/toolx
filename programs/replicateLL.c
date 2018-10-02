#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NODEMAX 10
#define ENDINDX (NODEMAX - 1)

#define rnd()  (rand()%100)
#define rnd2() (rand()%NODEMAX)

int debug = 1;

struct node {
    struct node * next;
    struct node * rand;
    int num;
};

struct node * arr[NODEMAX];

struct node * createlist () {
    struct node * this = NULL;
    struct node * next = NULL;
    struct node * head = NULL;

    for (int i = 0; i < NODEMAX; ++i) {
	this = (struct node *)malloc(sizeof(struct node));
	this->num = rnd();
	this->next = next;
	arr[ENDINDX - i] = this;

	next = this;
    }

    head = this;

    while (this) {
	this->rand = arr[rnd2()];
	this = this->next;
    }

    return head;
}


void printlist (struct node * head) {
    struct node * this = head;

    while (this) {
	printf("%2d [%2d] -> ", this->num, this->rand->num);
	this = this->next;
    }
    printf("|-\n");
}

struct node * replicate(struct node * head) {
    struct node * this      = head;
    struct node * next      = NULL;
    struct node * thisnew   = NULL;
    struct node **prevnew_p = NULL;
    struct node * headnew   = NULL;

    prevnew_p = &headnew;

    // first parse
    while (this) {
	next = this->next;

	thisnew = (struct node *)malloc(sizeof(struct node));
	thisnew->num = this->num;

	// fix pointers
	thisnew->next = NULL;
	thisnew->rand = this;
	this->next = thisnew;

	// connect with list
	*prevnew_p = thisnew;

	// move ahead
	prevnew_p = &(thisnew->next);
	this = next;
    }

    thisnew = headnew;

    // second parse
    while (thisnew) {
	thisnew->rand = thisnew->rand->rand->next;
	thisnew = thisnew->next;
    }

    return headnew;

}


int main() {
    struct node * headnew = NULL;
    struct node * head = createlist();
    printlist(head);

    headnew = replicate(head);
    printlist(headnew);

    return 0;
}



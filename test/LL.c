#include <stdio.h>
#include <stdlib.h>

struct node {
    int num;
    struct node * next;
};

int dataindex = 0;
int dataset[] = {83, 83, 83, 86, 86, 35, 83, 83, 83, 21, 62, 27, 90, 59, 83, 83, 83};

struct node * insert (struct node * headnode, int num) {
    struct node * this = headnode;
    struct node * new = NULL;
    new = (struct node *)malloc(sizeof(struct node));
    new->num = num;
    new->next = NULL;

    if(this == NULL) {
	this = new;
    } else {
	while(this->next != NULL)
	    this = this->next;
	this->next = new;
    }

    // returns the address of the newly allocated node.
    // Incase the caller needs to do additional modifications to this new node.
    // Hence in case of the first node it returns the headnode
    return new;
}

#define FORWARD  1
#define BACKWARD 2
// this function could be change to return the number of nodes displayed.
// good excercise in a recursive scenario
void displayrec(struct node * headnode, int dir) {
    if(!headnode) {
	// printf("List Empty\n");
	return; // no nodes
    }

    if(dir == FORWARD)
	printf("%d, ", headnode->num);

    // we dont need to NULL check headnode->next cuz the function checks it inside
    displayrec(headnode->next, dir);

    if(dir == BACKWARD)
	printf("%d, ", headnode->num);
}


struct node * reverse(struct node * headnode) {
    struct node * prevnode = NULL;
    struct node * nextnode = NULL;
    struct node * this = NULL;

    if(headnode)
	this = headnode;
    else
	return headnode;


    while(this) {
	nextnode = this->next;
	this->next = prevnode;
	prevnode = this;
	this = nextnode;
    }

    headnode = prevnode;
    return headnode;
}

void deleteMatches(struct node ** head_p, int match) {
    struct node * this = NULL;
    int val = 0;
    if(!(*head_p)) return;

    while((*head_p)) {
	val = (*head_p)->num;
	// printf("%d, ", (*head_p)->num);
	if(val == match) {
	    // printf("match\n");
	    // del this node and adjust pointers
	    this = (*head_p);
	    *head_p = this->next; // in case we are deleting a node, this step is already a next. skip nexting in this iteration
	    free(this);
	} else {
	    head_p = &((*head_p)->next);
	}
    }
    return;
}

#ifdef RANDOM
#define rnd() (rand()%100)
#else
#define rnd() (dataset[dataindex++])
#endif

int main() {
    struct node * head = NULL;

    printf("\n\n");
    head = insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());
    insert(head,rnd());

    displayrec(head, FORWARD); printf("\n");
    displayrec(head, BACKWARD); printf("\n");

    head = reverse(head);

    displayrec(head, BACKWARD); printf("\n");
    displayrec(head, FORWARD); printf("\n");


    deleteMatches(&head, 83);
    displayrec(head, FORWARD); printf("\n");

    return 0;

}



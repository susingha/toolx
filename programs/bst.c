
// This is a Binary Search Tree implementation
// All displayed would be in sorted order

// #warning: please compile along with tree.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#if 0
struct node {
    int num;
    struct node * lptr;
    struct node * rptr;
};
#endif

#include "draw_tree.h"


// Traverses a binary search tree and inserts at a leaf position
struct node *
insertiter(struct node * nodeHead, int num)
{
    struct node * nodeNew = (struct node *)malloc(sizeof(struct node));
    nodeNew->num = num;
    nodeNew->lptr = nodeNew->rptr = NULL;

    struct node * nodeThis = nodeHead;
    if(!nodeThis) {
        // this is the first node
        return nodeNew;
    } else {
        while (nodeThis) {
            if (num < nodeThis->num) {
                if (nodeThis->lptr) {
                    nodeThis = nodeThis->lptr;
                } else {
                    nodeThis->lptr = nodeNew;
                    break;
                }
            } else {
                if (nodeThis->rptr) {
                    nodeThis = nodeThis->rptr;
                } else {
                    nodeThis->rptr = nodeNew;
                    break;
                }
            }
        }
    }
    return nodeHead;
}

// Recursive traversal and insertion. More homogeneus code: Torvalds Style
// Passes address of head pointer during recursive call.
void
insertrec1(struct node ** nodeHead_, int num)
{
    if(*(nodeHead_) == NULL) {
        struct node * nodeNew = (struct node *)malloc(sizeof(struct node));
        nodeNew->num = num;
        nodeNew->lptr = nodeNew->rptr = NULL;
        *(nodeHead_) = nodeNew;
    } else {
        struct node * nodeThis = *(nodeHead_);
        if(num < nodeThis->num) {
            // go left and check for NULL
            insertrec1(&(nodeThis->lptr), num);
        }
        else if(num >= nodeThis->num) {
            // go right and check for NULL
            insertrec1(&(nodeThis->rptr), num);
        }
    }
}

// Recursive traversal and insertion. Messeir code then insertrec1
// Passes the head pointer during recursive call.
struct node *
insertrec2(struct node * nodeHead, int num)
{
    struct node * nodeNew = NULL;

    if(!nodeHead) {
#ifndef ALLOC
        nodeNew = (struct node *)malloc(sizeof(struct node));
        nodeNew->num = num;
        nodeNew->lptr = nodeNew->rptr = NULL;
#endif
        return nodeNew;
    } else {
        struct node * nodeThis = nodeHead;
        if(num < nodeThis->num) {
            if(nodeThis->lptr) {
                insertrec2(nodeThis->lptr, num);
            } else {
#ifndef ALLOC
                nodeNew = (struct node *)malloc(sizeof(struct node));
                nodeNew->num = num;
                nodeNew->lptr = nodeNew->rptr = NULL;
#endif
                nodeThis->lptr = nodeNew;
            }
        } else if(num >= nodeThis->num) {
            if(nodeThis->rptr) {
                insertrec2(nodeThis->rptr, num);
            } else {
#ifndef ALLOC
                nodeNew = (struct node *)malloc(sizeof(struct node));
                nodeNew->num = num;
                nodeNew->lptr = nodeNew->rptr = NULL;
#endif
                nodeThis->rptr = nodeNew;
            }
        }
    }

    return nodeHead;
}


// Needs work. Unused

#ifdef UNUSED
void deleteiter(struct node * head, int find) {
    int prev;
    struct node *delnode = NULL, *pred = NULL;
    prev = displayiter(head, 63, delnode, pred);

    // we are trying to delete 63.
    // delnode points to 63 and pred points to predecessor of 63

    if(delnode->lptr == NULL && delnode->rptr == NULL) {
	// we need the parent pointer too and whether this is a left or right child
    }
}
#endif



// returns the lowest common ancestor of two nodes in the tree
struct node * lca(struct node * head, int i, int j) {
    struct node *left = NULL, *right = NULL, *self = NULL;

    if (!head) {
        return NULL;
    }

    if (head->lptr) {
        left = lca(head->lptr, i, j);
    }

    if (head->num == i || head->num == j) {
        self = head;
    }

    if (head->rptr) {
        right = lca(head->rptr, i, j);
    }

    if (self)
        return self;
    if (left && right)
        return head;
    if (left)
        return left;
    if (right)
        return right;

    return NULL;
}



// Inorder Traversal and display
// this function could be change to return the number of nodes displayed.
// good excercise in a recursive scenario

void displayrec(struct node * nodeHead, int outer)
{
    struct node *nodeThis;

    if(!nodeHead)
        return;

    if(outer) {
        printf("Using tree recursion inorder\n");
    }

    nodeThis = nodeHead;
    displayrec(nodeThis->lptr, FALSE);
    printf("%d, ", nodeThis->num);
    displayrec(nodeThis->rptr, FALSE);

    if(outer)
        printf("\n");
}


// Core Idea: Use a stack
// push, if left, go left
// pop,  if right, go right
// exit when stack is empty

#define PUSH(x) arr[++top] = x;
#define POP() arr[top]; if (top) top--;

int displayiter(struct node * nodeHead, int find /*, struct node ** delnode, struct node ** pred */)
{
    // INIT STACK
    struct node * arr[100];
    unsigned int top = 0;
    arr[top] = NULL;

    struct node *this = NULL;
    unsigned int new = 1; // new is a marker to understand after a continue whether to traverse left or right
                          // new 1 means its a new traverse on a fresh subtree so go left, else go right
    int prev = 0, found = 0;
    int done = FALSE;

    // this is in inorder
    this = nodeHead;

#define VERSION_3
#if defined(VERSION_1)
    printf("Using tree iteration v1\n");
    while (!done) {
        if (this) {
            PUSH(this);
            this = this->lptr;
            continue;
        }

        this = POP();

        if (this) {
            printf("%d, ", this->num);
            this = this->rptr;
        } else {
            done = TRUE;
        }
    }
#elif defined(VERSION_2)
    printf("Using tree iteration v2\n");
    while (!done) {
        if (this) {
            if (this->lptr) {
                PUSH(this);
                this = this->lptr;
                continue;
            }

            printf("%d, ", this->num);
            this = this->rptr;
            continue;
        }

        this = POP();

        if (!this) {
            done = TRUE;
            continue;
        } else {
            printf("%d, ", this->num);
            this = this->rptr;
        }
    }
#elif defined(VERSION_3)
    printf("Using tree iteration v3: write yourself\n");

    this = nodeHead;
    while (TRUE) {
	while (this) {
	    PUSH(this);
	    this = this->lptr;
	}

	this = POP();
	if (!this) break;

	printf("%d, ", this->num);

	if (this->rptr) {
	    this = this->rptr;
	    continue;
	} else {
	    this = NULL;
	    continue;
	}
    }

#endif

    printf("\n");
    return 0;
}

#define rnd() (rand()%100)
int main()
{
    struct node * head = NULL, *tmp;
    int i, j, k = -1;
#if 0
    srand(time(NULL));
#endif

    displayrec(head, TRUE);
#if 0
    head = insertiter(head, 5);
    insertiter(head, 1);
    insertiter(head, 8);
#endif
    insertrec1(&head, rnd());
    insertiter(head, rnd());
    insertrec2(head, rnd());
    insertiter(head, rnd());
    insertiter(head, rnd());
    insertiter(head, rnd());
    insertiter(head, rnd());
    insertiter(head, rnd());
    insertiter(head, rnd());
    insertrec2(head, 7);
    insertrec1(&head, 5);
    insertrec2(head, 6);
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    displayrec(head, TRUE);
    displayiter(head, 5);
    draw_tree(head);

    while (1) {
        printf("i: ");
        scanf("%d", &i);
        printf("j: ");
        scanf("%d", &j);

        tmp = lca(head, i, j);
        if (tmp) k = tmp->num;
        printf("lca of %d and %d is %d\n", i, j, k);
    }

    printf("\n");
    return 0;
}




// Try to recreate a tree from an inorder and preorder traversal
// http://www.geeksforgeeks.org/construct-tree-from-given-inorder-and-preorder-traversal/
// recursive. find iterative



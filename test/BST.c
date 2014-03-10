/* This is a Binary Search Tree implementation
 * All displayed would be in sorted order
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct node
{
    int num;
    struct node * lptr;
    struct node * rptr;
};


struct node * insert(struct node * nodeHead, int num)
{
    struct node * nodeNew = (struct node *)malloc(sizeof(struct node));
    nodeNew->num = num;
    nodeNew->lptr = nodeNew->rptr = NULL;
    // printf("\nsup:0 inserting %d", num);

    struct node * nodeThis = nodeHead;
    if(!nodeThis)
    {
        // this is the first node
        return nodeNew;
    }
    else
    {
        // first node exists. go to the required place and fix it there
        do
        {
            if(num < nodeThis->num)
            {
                // go left and check for NULL
                if(nodeThis->lptr)
                {
                    nodeThis = nodeThis->lptr;
                }
                else
                {
                    nodeThis->lptr = nodeNew;
                    break;
                }
            }
            else if(num >= nodeThis->num)
            {
                // go right and check for NULL
                if(nodeThis->rptr)
                {
                    nodeThis = nodeThis->rptr;
                }
                else
                {
                    nodeThis->rptr = nodeNew;
                    break;
                }
            }
        }
        while (1);
    }
    return nodeHead;
}

void insertrec1(struct node ** nodeHead_, int num)
{
    if(*(nodeHead_) == NULL)
    {
        struct node * nodeNew = (struct node *)malloc(sizeof(struct node));
        nodeNew->num = num;
        nodeNew->lptr = nodeNew->rptr = NULL;
        *(nodeHead_) = nodeNew;
    }
    else
    {
        struct node * nodeThis = *(nodeHead_);
        if(num < nodeThis->num)
        {
            // go left and check for NULL
            insertrec1(&(nodeThis->lptr), num);
        }
        else if(num >= nodeThis->num)
        {
            // go right and check for NULL
            insertrec1(&(nodeThis->rptr), num);
        }
    }

}

struct node * insertrec2(struct node * nodeHead, int num)
{
    if(!nodeHead)
    {
        struct node * nodeNew = (struct node *)malloc(sizeof(struct node));
        nodeNew->num = num;
        nodeNew->lptr = nodeNew->rptr = NULL;
        return nodeNew;
    }
    else
    {
        struct node * nodeThis = nodeHead;
        if(num < nodeThis->num)
        {
            if(nodeThis->lptr)
            {
                insertrec2(nodeThis->lptr, num);
            }
            else
            {
                struct node * nodeNew = (struct node *)malloc(sizeof(struct node));
                nodeNew->num = num;
                nodeNew->lptr = nodeNew->rptr = NULL;
                nodeThis->lptr = nodeNew;
            }
        }
        else if(num >= nodeThis->num)
        {
            if(nodeThis->rptr)
            {
                insertrec2(nodeThis->rptr, num);
            }
            else
            {
                struct node * nodeNew = (struct node *)malloc(sizeof(struct node));
                nodeNew->num = num;
                nodeNew->lptr = nodeNew->rptr = NULL;
                nodeThis->rptr = nodeNew;
            }
        }
    }
}


// this function could be change to return the number of nodes displayed.
// good excercise in a recursive scenario
void displayrec(struct node * nodeHead)
{
    // this is in inorder
    if(!nodeHead) {
        return;
    }

    struct node * nodeThis = nodeHead;

    displayrec(nodeThis->lptr);
    printf("%d, ", nodeThis->num);

    displayrec(nodeThis->rptr);

}


void delete(struct node * head, int find) {
    int prev;
    struct node * delnode = NULL, pred = NULL;
    prev = displayiter(head, 63, delnode, pred);

    // we are trying to delete 63.
    // delnode points to 63 and pred points to predecessor of 63

    if(delnode->lptr == NULL && delnode->rptr == NULL) {
	// we need the parent pointer too and whether this is a left or right child

    }
}


int displayiter(struct node * nodeHead, int find, struct node ** delnode, struct node ** pred)
{
    struct node * this = NULL;
    struct node * arr[100];
    signed int top = -1;
    unsigned int new = 1; // new is a marker to understand after a continue whether to traverse left or right
                          // new 1 means its a new traverse on a fresh subtree so go left, else go right
    int prev = 0, found = 0;

    // this is in inorder
    this = nodeHead;
    while(this) {
	if(this->lptr && new) {
	    arr[++top] = this;
	    this = this->lptr;
	    new = 1;
	    continue;
	} else {

	    printf("%d, ", this->num);
	    if(this->num != find && !found) {
		prev = this->num;
		*pred = this;
	    } else {
		found = 1;
		*delnode = this;
	    }

	    if(this->rptr) {
		this = this->rptr;
		new = 1;
		continue;
	    } else {
		this = arr[top--];
		if(top > -2) { // just signifies that the last elem of the stack has been popped out
		    new = 0;
		    continue;
		} else {
		    break;
		}
	    }
	}
    }

    if(pred->num != prev || delnode->num != find) {
	printf("\nsup: WARNING: something wrong. predecessor not computed properly");
    }
    return prev;
}

#define rnd() (rand()%100)
int main()
{
    struct node * head = NULL;
    displayrec(head);
    printf("\n\n");
//    head = insert(head, 5);
//    insert(head, 1);
//    insert(head, 8);
    head = insertrec2(head, rnd());
    insert(head, rnd());
    insertrec2(head, rnd());
    insert(head, rnd());
    insert(head, rnd());
    insertrec1(&head, rnd());
    insert(head, rnd());
    insert(head, rnd());
    insert(head, rnd());
    insert(head, rnd());
    insertrec2(head, 7);
    insertrec1(&head, 5);
    insertrec2(head, 6);
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    insertrec2(head, rnd());
    printf("\n--- display --- \n");
    displayrec(head);
    printf("\n--- display --- \n");
    displayiter(head, 5);
    printf("\n");

    return 0;
}

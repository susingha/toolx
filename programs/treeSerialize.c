/*
 * Given the root to a binary tree,
 * implement serialize(root), 
 * which serializes the tree into a string,
 * and deserialize(s), which deserializes
 * the string back into the tree.
 * node = Node('root', Node('left', Node('left.left')), Node('right'))
 * assert deserialize(serialize(node)).left.left.val == 'left.left'
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "template.h"
#include "draw_tree.h"

void getarr(int arr[], int n)
{
    while (n--) arr[n] = getrandom();
    printf("\n");
}

void showarr(int arr[], int n)
{
    int i = 0;
    while (n--) printf("%d, ", arr[i++]);
    printf("\n");
}

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

#define XOPEN  ((char)200)
#define XCOMMA ((char)201)
#define XCLOSE ((char)202)
char serialized[1024];
int seridx = 0;
int serlen = 0;

void createSerialize(struct node * head)
{
    if (!head)
	return;

    serialized[seridx++] = head->num;
    serialized[seridx++] = XOPEN;        // (

    if (head->lptr)
	createSerialize(head->lptr);

    serialized[seridx++] = XCOMMA;       // ,

    if (head->rptr)
	createSerialize(head->rptr);

    serialized[seridx++] = XCLOSE;       // )
}

struct node * createDeserialized(struct node * parent)
{
    struct node ** refholder = NULL;
    struct node *  curr = NULL;
    int done = FALSE;
    int i = 0;

    while(!done && seridx < serlen) {
	i = seridx++;
	switch(serialized[i]) {
	    case XOPEN:
		if (parent)
		    refholder = &(parent->lptr);
		break;
	    case XCOMMA:
		if (parent)
		    refholder = &(parent->rptr);
		break;
	    case XCLOSE:
		done = TRUE;
		break;
	    default:
		curr = (struct node *) malloc(sizeof(struct node));
		curr->num = serialized[i];
		curr->lptr = NULL;
		curr->rptr = NULL;

		createDeserialized(curr);

		if (refholder) {
		    *refholder = curr;
		} else {
		    done = TRUE;
		    return curr;
		}
	}
    }
    return NULL;
}

int inorder[1024];
int inindex = 0;
int prorder[1024];
int prindex = 0;

void printInorder(struct node * head)
{
    if (!head)
	return;

    if (head->lptr)
	printInorder(head->lptr);

    inorder[inindex++] = head->num;

    if (head->rptr)
	printInorder(head->rptr);
}

void printProrder(struct node * head)
{
    if (!head)
	return;

    prorder[prindex++] = head->num;

    if (head->lptr)
	printProrder(head->lptr);

    if (head->rptr)
	printProrder(head->rptr);
}

int stackindex = 0;
struct node * stack[1024];
void printIterative(struct node * head)
{
    struct node * curr = head;
    stack[stackindex++] = NULL;
    int done = FALSE;

    if (!curr)
	return;

    while(!done) {
	do {
	    stack[stackindex++] = curr;  // push
	    curr = curr->lptr;
	} while (curr);

	while ((curr = stack[--stackindex]) != NULL) {  // pop
	    printf("%d, ", curr->num);

	    if (curr->rptr) {
		curr = curr->rptr;
		break;
	    } else {
		continue;
	    }
	}

	if (!curr) {
	    done = TRUE;
	    break;
	}
    }
}

int arr[1024];
int len = 10;

int main (int argc, char *argv[])
{
    int i;
    getarr(arr, len);
    printf("Input\n");
    showarr(arr, len);
    printf("\n\n");


    struct node * head = NULL;;
    struct node * headnew = NULL;;
    for (i = 0; i < len; ++i)
	insertrec1(&head, arr[i]);

    printf("Inorder\n");
    printInorder(head);
    showarr(inorder, inindex);
    printf("\n\n");

    printf("Preorder\n");
    printProrder(head);
    showarr(prorder, prindex);
    printf("\n\n");

    printf("Iterative Inorder\n");
    printIterative(head);
    printf("\n\n");

    draw_tree(head);

    /* Serialize */
    createSerialize(head);
    serlen = seridx;

    printf("Serialized:\n");
    for (i = 0; i < serlen; ++i) {
	switch(serialized[i]) {
	    case XOPEN:
		printf("(");
		break;
	    case XCOMMA:
		printf(",");
		break;
	    case XCLOSE:
		printf(")");
		break;
	    default:
		printf("%d", serialized[i]);
	}
    }
    printf("\n\n");

    seridx = 0;
    inindex = 0;
    prindex = 0;

    /* Deserialize */
    headnew = createDeserialized(NULL);

    printf("Inorder Deserialized\n");
    printInorder(headnew);
    showarr(inorder, inindex);
    printf("\n\n");

    printf("Preorder Deserialized\n");
    printProrder(headnew);
    showarr(prorder, prindex);
    printf("\n\n");

    printf("Iterative Inorder Deserialized\n");
    printIterative(headnew);
    printf("\n\n");

    draw_tree(headnew);

    return 0;
}


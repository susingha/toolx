/*
 * A unival tree (which stands for "universal value")
 * is a tree where all nodes under it have the same value.
 *
 * Given the root to a binary tree,
 * count the number of unival subtrees.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "template.h"
#include "draw_tree.h"

#if 0
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#endif


#define INVALID (-1)

struct node * createTree(int arr[], int maxidx, int idx)
{
    if (idx > maxidx)
	return NULL;
    if (arr[idx] == INVALID)
	return NULL;

    struct node * new = (struct node *)malloc(sizeof(struct node));
    new->num = arr[idx];
    new->lptr = createTree(arr, maxidx, (2*idx)+1);
    new->rptr = createTree(arr, maxidx, (2*idx)+2);
    return new;
}

int unival = 0;
int univaltree(struct node * head)
{
    int lok = TRUE,
	rok = TRUE;

    if (head->lptr) {
	if (univaltree(head->lptr) && head->lptr->num == head->num) {
	    lok = TRUE;
	} else {
	    lok = FALSE;
	}
    }

    if (head->rptr) {
	if (univaltree(head->rptr) && head->rptr->num == head->num) {
	    rok = TRUE;
	} else {
	    lok = FALSE;
	}
    }

    if (lok && rok) {
	unival++;
	return TRUE;
    }

    return FALSE;
}

int main (int argc, char *argv[])
{
    struct node * head;
    printargs(argc, argv);


#if 0
    int tree[] = {1,1,1};
    int tree[] = {0,1,0,-1,-1,1,0,-1,-1,-1,-1,1,1,-1,-1};
    int tree[] = {5,1,5,5,5,-1,5};
    int tree[] = {5,4,5,4,4,-1,5};
#endif
    int tree[] = {5,4,5,4,4,-1,5};
    int len = sizeof(tree)/sizeof(tree[0]);
    head = createTree(tree, len-1, 0);
    
    draw_tree(head);

    trackon();

    univaltree(head);
    printf("Unival trees: %d\n", unival);

    trackoff(); trackprint();

    return 0;
}




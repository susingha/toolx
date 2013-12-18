#include <stdio.h>
#include <stdlib.h>


struct node
{
    int num;
    struct node * lptr;
    struct node * rptr;
};

struct node * head = NULL;

struct node * insert(struct node * nodeHead, int num)
{
    struct node * nodeNew = (struct node *)malloc(sizeof(struct node));
    nodeNew->num = num;
    nodeNew->lptr = nodeNew->rptr = NULL;

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

void insertrec(struct node ** nodeHead_, int num)
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
            insertrec(&(nodeThis->lptr), num);
        }
        else if(num >= nodeThis->num)
        {
            // go right and check for NULL
            insertrec(&(nodeThis->rptr), num);
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

void displayrec(struct node * nodeHead)
{
    // this is in inorder
    if(!nodeHead)
    {
        // printf("no nodes\n");
        return;
    }

    struct node * nodeThis = nodeHead;
    displayrec(nodeThis->lptr);
    printf("%d\n", nodeThis->num);
    displayrec(nodeThis->rptr);
}

void displayiter(struct node * nodeHead) // buggy
{
    int * arr[100];
    int top = -1;
    // this is in inorder
    while(nodeHead)
    {
        if(nodeHead != NULL)
        {
            // printf("here1\n");
            // push nodeHead;
            arr[++top] = (int *)nodeHead;
            nodeHead = nodeHead->lptr;

        }
        // printf("here2\n");
        if(nodeHead == NULL)
        {
            // printf("here3\n");
            // pop nodeHead;
            nodeHead = (struct node *)arr[top--];
            printf("%d\n", nodeHead->num);
            nodeHead = nodeHead->rptr;
        }
    }

}

#define rnd() (rand()%100)
int main()
{
    displayrec(head);
    printf("\n\n");
    head = insertrec2(head, rnd());
    insert(head, rnd());
    insertrec2(head, rnd());
    insert(head, rnd());
    insert(head, rnd());
    insertrec(&head, rnd());
    insert(head, rnd());
    insert(head, rnd());
    insert(head, rnd());
    insert(head, rnd());

    insertrec2(head, 7);
    insertrec(&head, 5);
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
    printf("--- display rec --- \n");
    displayrec(head);

    return 0;
}

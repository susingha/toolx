#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//query and match contains the entire line

int compare(char query[], char match[], int i, int j)
{
   int k= 0;
   int l= 0;
   int flag;
    //compare
    while(k < (i -1))
    {
	printf("\nquery[%d] is %c ", k, query[k]);
	while(query[k] != match[l])
	{
	    printf("\nmatch[%d] is %c ", l, match[l]);
	    if(l == i)
		   break;
	    else
	    {
		l++;
		flag = 0;
	    }
	}
	l++;
	printf("\n query[%d] %c matches match[%d] %c", k, query[k], l, match[l]);
	flag = 1;
	printf("\n flag = %d", flag);
	if (k = 0 && match[l-1] != '\0')
	{
	    if (l == j)
		break;
	    else
	    {
		flag = 0;
	    }
	}
	l++;
	k++;
    }
    return flag;

}


void main()
{
/*  FILE *ifp, *ofp;
    char *mode = "r";
    char outputFilename[] = "out.list";

    ifp = fopen("in.list", mode);

    if (ifp == NULL) {
	fprintf(stderr, "Can't open input file in.list!\n");
	exit(1);
    }

    ofp = fopen(outputFilename, "w");

    if (ofp == NULL) {
	fprintf(stderr, "Can't open output file %s!\n",
		outputFilename);
	exit(1);
    }
*/

    char a[]="cat";
    char b[]="this cat";
    int flag= 0;
    flag = compare(a, b, sizeof(a), sizeof(b));
    if (flag == 1)
    {
	printf ("\ntrue\n");
    }
    else
	printf("\nfalse\n");
}

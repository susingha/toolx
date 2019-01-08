/*
 * """
 * Suppose we represent our file system by a string in the following manner:
 * 
 * The string "dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext" represents:
 * 
 * dir
 *     subdir1
 *     subdir2
 *         file.ext
 * The directory dir contains an empty sub-directory subdir1 and 
 * a sub-directory subdir2 containing a file file.ext.
 * 
 * The string "dir\n\tsubdir1\n\t\tfile1.ext\n\t\tsubsubdir1\n\tsubdir2\n\t\tsubsubdir2\n\t\t\tfile2.ext" represents:
 * 
 * dir
 *     subdir1
 *         file1.ext
 *         subsubdir1
 *     subdir2
 *         subsubdir2
 *             file2.ext
 * The directory dir contains two sub-directories subdir1 and subdir2.
 * subdir1 contains a file file1.ext and an empty second-level sub-directory subsubdir1. 
 * subdir2 contains a second-level sub-directory subsubdir2 containing a file file2.ext.
 * 
 * We are interested in finding the longest (number of characters) absolute path to a file within our file system. 
 * For example, in the second example above, 
 * the longest absolute path is "dir/subdir2/subsubdir2/file2.ext", 
 * and its length is 32 (not including the double quotes).
 * 
 * Given a string representing the file system in the above format, 
 * return the length of the longest absolute path to a file in the abstracted file system. 
 * If there is no file in the system, return 0.
 * 
 * Note:
 * The name of a file contains at least a period and an extension.
 * The name of a directory or sub-directory will not contain a period.
 * """
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "template.h"
#include <string.h>

#if 0
#include <stdint.h>
#include <sys/types.h>
#endif


#if 0
char dirtree[] = "dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext";
#else
char dirtree[] = "dir\n\tsubdir1\n\t\tfile1.ext\n\t\tsubsubdir1\n\tsubdir2\n\t\tsubsubdir2\n\t\t\tfile2.ext";
#endif

struct inode {
    char * filename;
    int    namelen;
};

#define MAXDEPTH 10
struct inode pathName[MAXDEPTH];
struct inode pathNameMax[MAXDEPTH];
int levelMax = 0, maxlen = 0;


void
printPath(struct inode * path, int level)
{
    int i;
    for (i = 0; i <= level; ++i)
    {
	printf("%.*s", path[i].namelen, path[i].filename);
	printf("/");
    }
    printf("\n");
}

void
saveNewPath(int level)
{
    levelMax = level;
    for (int i = 0; i <= level; ++i) {
	pathNameMax[i] = pathName[i];
    }
}

void
updatePath(int level)
{
    int i, totallen = 0;
    for (i = level; i >= 0; --i) {
	totallen += pathName[i].namelen;
    }

    if (totallen > maxlen) {
	maxlen = totallen;
	saveNewPath(level);
    }
}

void
addCount(int b, int e, int level, int file)
{

    pathName[level].namelen = e-b+1;
    pathName[level].filename = &dirtree[b];

    if (file) {
	updatePath(level);
    }
}

int
isChar(char c)
{
    if ('a' < c && c < 'z') return TRUE;
    if ('A' < c && c < 'Z') return TRUE;
    if ('0' < c && c < '9') return TRUE;
    return FALSE;
}

int
findNL(int b, int *file)
{
    *file = FALSE;
    while (dirtree[b] != '\n' && dirtree[b] != '\0') {
	if (dirtree[b] == '.')
	    *file = TRUE;
	b++;
    }
    return b-1;
}

int
findLevel(int b)
{
    int level = 0;

    b--;
    while (!isChar(dirtree[b]) && b != -1) {
	if (dirtree[b] == '\t')
	    level++;
	b--;
    }

    return level;
}

int
findName(int e)
{
    e++;
    while (!isChar(dirtree[e]) && dirtree[e] != '\0')
	e++;

    if (dirtree[e] == '\0')
	return -1;
    else
	return e;
}


int main (int argc, char *argv[])
{
    printargs(argc, argv);
    int i = 0, beginName = 0, endName = 0;
    int file = 0, level = 0;

    printf("%s\n", dirtree);
    int len = strlen(dirtree);

    while (beginName != -1) {
	endName = findNL(beginName, &file);  // looks for \n and returns end of name. Also returns dir or file
	level = findLevel(beginName);        // looks back to get the number of \ts to get the level. for top level folder / file, will return 0

	addCount(beginName, endName, level, file);

	beginName = findName(endName);       // returns the next index at which it finds a dir / file name. -1 if its the end of string
    }

    printPath(pathNameMax, levelMax);

    return 0;
}




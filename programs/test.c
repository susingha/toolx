#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define ADD 0
#define SUB 1

int debug = 0;

int ascix(char c)
{
    return (c - '0');
}


int eval (int num1, int op, int num2) {
    int result = 0;

    if (op == ADD) {
	result = num1 + num2;
	if (debug) printf("%d + %d = %d\n", num1, num2, result);
	return result;
    }

    if (op == SUB) {
	result = num1 - num2;
	if (debug) printf("%d - %d = %d\n", num1, num2, result);
	return result;
    }

    return 0;
}


int getnum (char * c, int i, int * nump) {
    int num = 0;

    if (c[i] == '\0') {
	return i;
    }

    while ('0' <= c[i] && c[i] <= '9') {
	num = (num * 10) + ascix(c[i]);
	i++;
    }

    if (debug) printf("found new num = %d\n", num);
    *nump = num;

    return i - 1;
}

int calculate (char * c) {
    int i;
    int len = strlen(c);
    char cur;
    int result = 0;
    int op = ADD;
    int num = 0;

    for (i = 0; i < len; ++i) {
	cur = c[i];

	if (cur == ' ' ||
            cur == '(' ||
	    cur == ')')
	    continue;

	if (cur == '+') {
	    op = ADD;
	    continue;
	}

	if (cur == '-') {
	    op = SUB;
	    continue;
	}

	i = getnum(c, i, &num);

	result = eval(result, op, num);
	num = 0;

    }

    return result;
}



pid_t pid;

int main(int argc, char *argv[]) {

    pid = fork();
    if (pid == 0) {
	printf("child: sleeping\n");
	sleep(15);
	printf("child: exit()\n");
	exit(0);
    } else {
	printf("parent: sleeping the child process is %d\n", pid);
	sleep(20);
	printf("parent: exit()\n");
    }

    return 0;

#if 0
    int i;
    if (debug) printf("argc: %d\n", argc);
    for (i = 0; i < argc; ++i) {
	if (debug) printf("argv[%d]: %s\n", i, argv[i]);
    }
#endif

    char string[1024];
    scanf("%[^\n]s", string);
    if (debug) printf("%s\n", string);
    return  calculate(string);
}

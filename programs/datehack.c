
/*
 * http://hackerboss.com/overriding-system-functions-for-fun-and-profit/
 *
 * date
 * gcc -Wall -fPIC -DPIC -c datehack.c
 * ld -shared -o datehack.so datehack.o -ldl
 * export LD_PRELOAD=./datehack.so
 * date
 *
 */

#define _GNU_SOURCE
#include <time.h>
#include <dlfcn.h>
#include <stdio.h>

struct tm *(*orig_localtime)(const time_t *timep);

struct tm *localtime(const time_t *timep)
{
    time_t t = *timep - 60 * 60 * 24;
    return orig_localtime(&t);
}

    void
_init(void)
{
    printf("Loading hack.\n");
    orig_localtime = dlsym(RTLD_NEXT, "localtime");
}

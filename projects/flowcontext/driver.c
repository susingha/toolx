
#include <stdio.h>
#include <stdlib.h>

// ============================================================================= //

#include "flowcontext.h"

#if 1
void fun5() {
    PRINTFLOWCONTEXT();
}

void fun4() {
    PRINTFLOWCONTEXT();
    fun5();
}

void fun3() {
    PRINTFLOWCONTEXT();
    fun4();
}

void fun2() {
    PRINTFLOWCONTEXT();
    fun3();
}

void fun1() {
    INITFLOWCONTEXT();
    PRINTFLOWCONTEXT();
    fun2();
    DEINITFLOWCONTEXT();
}
void recurse(int d) {
    if (d > 0) {
	recurse(d-1);
    }

    if (d == 0) {
	fun1();
    }
}
int main() {
    recurse(100);
    return 0;
}
#else
int main() {
#if 0
    unsigned int i = 0;
    unsigned int j = backtrace();
    printf("sup: total frame: %u\n", j);

    i = 0;
    printf("sup: %u - %p\n", i, my__builtin_frame_address(i)); i++;
    printf("sup: %u - %p\n", i, my__builtin_frame_address(i)); i++;
    printf("sup: %u - %p\n", i, my__builtin_frame_address(i)); i++;
    printf("sup: %u - %p\n", i, my__builtin_frame_address(i)); i++;

    i = 0;
    printf("sup: %u - %p\n", i, my__builtin_return_address(i)); i++;
    printf("sup: %u - %p\n", i, my__builtin_return_address(i)); i++;
    printf("sup: %u - %p\n", i, my__builtin_return_address(i)); i++;
#endif

    INITFLOWCONTEXT();
    PRINTFLOWCONTEXT();
    return 0;
}
#endif

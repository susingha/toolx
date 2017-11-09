#include <stdio.h>


extern int foo;

void funny() {
    foo = 9;
    return;
}

void fun() {
    foo = 9;
    funny();
    return;
}

int main() {
    foo = 8;
    fun();
    return foo;

}




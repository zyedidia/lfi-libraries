#include <stdio.h>

_Thread_local int x;

void foo() {
    printf("x: %d\n", x++);
}

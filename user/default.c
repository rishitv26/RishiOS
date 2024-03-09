#include "usrlib/stdio.h"

// basic user program that runs in user mode...
int main(void) {
    printf("type something here:\n");
    while (true) sleepu(100);
    return 0;
}
#include "usrlib/stdio.h"

// basic user program that runs in user mode...
int main(void) {
    printf("type something here:\n");
    sleepu(1000);
    clearu();
    printf("type whatever your little heart desires.\n");
    return 0;
}
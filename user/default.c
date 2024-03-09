#include "usrlib/stdio.h"

#define DELAY 4000000 // hertz of the cpu
// basic user program that runs in user mode...
int main(void) {
    int counter = 0;
    while (true) {
        sleepu(1000);
        char *p = (char*)0xffff800000200200;
        *p = 1;

        // should not print.
        printf("proccess2 has %d ticks.\n", counter);
        counter++;
    }
    return 0;
}
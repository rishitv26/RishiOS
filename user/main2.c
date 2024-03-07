#include "usrlib/stdio.h"

#define DELAY 4000000 // hertz of the cpu
// basic user program that runs in user mode...
int main(void) {
    int counter = 0;
    while (true) {
        for (int i = 0; i < 400000; ++i) sleepu(DELAY);
        printf("proccess2 has %d ticks.\n", counter);
        counter++;
    }
    return 0;
}
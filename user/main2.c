#include "usrlib/stdio.h"

#define DELAY 10000000
// basic user program that runs in user mode...
int main(void) {
    // int64_t counter = 0;
    // while (true) {
    //     if (counter % 100000 == 0)
    //         printlnf("proccess2: %d", counter);
    //     counter++;
    // }
    int counter = 0;
    while (true) {
        printf("proccess2 has %d ticks.\n", counter * DELAY);
        counter++;
        sleep(DELAY);
    }
    return 0;
}
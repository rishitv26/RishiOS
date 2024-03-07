#include "usrlib/lib.h"

#define DELAY 40000000
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
        sleep(DELAY);
        printf("proccess1 has %d ticks.\n", counter * DELAY);
        counter++;
    }
    return 0;
}
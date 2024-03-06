#include "usrlib/stdio.h"

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
        printf("proccess2 has %d secs.\n", counter);
        counter++;
        sleep(1000);
    }
    return 0;
}
#include "usrlib/stdio.h"

// basic user program that runs in user mode...
int main(void) {
    int64_t counter = 0;
    while (true) {
        if (counter % 100000 == 0)
            printf("proccess2: %d\n", counter);
        counter++;
    }
    return 0;
}
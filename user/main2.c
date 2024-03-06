#include "usrlib/stdio.h"

int main(void) {
    uint64_t counter = 0;

    while (true) {
        if (counter % 100000 == 0) {
            printf("proccess2: %d\n", counter);
        }
        counter++;
    }

    return 0;
}

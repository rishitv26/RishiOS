#include "../usrlib/lib.h"

// Garbage collects all killed proccesses with thier memory.
int main(void) {
    while (true) { // not neccessary, but for safety.
        cleanup();
    }
    return 0;
}
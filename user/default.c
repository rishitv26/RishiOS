#include "usrlib/stdio.h"

// basic user program that runs in user mode...
int main(void) {
    printf("type something here:\n");
    sleepu(1000);
    clearu();
    printf("type whatever your little heart desires.\n");
    char str[] = {0};
    while (true) {
        str[0] = keyboard_readu();
        printf("%s", str);
    }
    return 0;
}
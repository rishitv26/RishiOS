#include "usrlib/lib.h"

// basic user program that runs in user mode...
int main(void) {
    const char* string = "hello";
    size_t size = strlen(string);
    printf("The size of: \"%s\" is %d\n", string, size);
    
    return 0;
}
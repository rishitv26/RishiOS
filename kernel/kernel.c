#include "common.h"
#include "screen/monitor.h"

struct boot {};

extern int main (struct boot *boot_ptr)
{
    // boot proccess:
    print("[*] ", 0, 12); print("Booted into Kernel...\n", 0, 3); // into c entry point
    print(itoa(1927, 2), 0, 15);
    return 0; // exit code
}
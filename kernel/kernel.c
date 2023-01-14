#include "common.h"
#include "screen/monitor.h"

extern int main ()
{
    // boot proccess:
    print("[*] ", 0, 12); print("Booted into Kernel...\n", 0, 3); // into c entry point
    init_idt_tables();
    return 0; // exit code
}
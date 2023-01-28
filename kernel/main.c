#include "lib/head.h"
#include "interrupt/inter.h"
#include "mem/mem.h"

struct freemem MEM_MAP[50];
uint64_t pages_end_ptr;

void main(void)
{
    const char* welcome_mes = "[*] WELCOME TO RishiOS, initializing other computer processes...\n";
    printk("%s\n", welcome_mes);
    
    init_idt(); // initialize interrupts...
    init_mem(MEM_MAP, &pages_end_ptr); // initialize memory map
    printk("%x", pages_end_ptr);
    // end of kernel
}

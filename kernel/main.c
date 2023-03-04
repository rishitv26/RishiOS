#include "lib/head.h"
#include "interrupt/inter.h"
#include "mem/mem.h"
#include "proccess/proccess.h"

struct freemem MEM_MAP[50];

void kernel(void)
{
    const char* welcome_mes = "[*] WELCOME TO RishiOS, initializing other computer processes...\n";
    printk("%s\n", welcome_mes);
    
    init_idt(); // initialize interrupts...
    init_mem(MEM_MAP); // initialize memory map...

    printk("Spawning first proccess...\n");
    init_proccess(); 
    launch();

    // end of kernel
}

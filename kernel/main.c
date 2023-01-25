#include "lib/head.h"
#include "interrupt/inter.h"
#include "mem/mem.h"

void main(void)
{
    const char* welcome_mes = "[*] WELCOME TO RishiOS, initializing other computer processes...\n";
    printk("%s\n", welcome_mes);
    
    init_idt();
    init_mem();
    // end of kernel
}

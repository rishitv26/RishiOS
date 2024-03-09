#include "lib/head.h"
#include "interrupt/inter.h"
#include "mem/mem.h"
#include "proccess/proccess.h"
#include "syscalls/syscall.h"

struct freemem MEM_MAP[50];

void kernel(void)
{
    const char* welcome_mes = "[*] WELCOME TO RishiOS, initializing other components...\n";
    printk("%s\n", welcome_mes);
    
    init_idt(); // initialize interrupts...
    init_mem(MEM_MAP); // initialize memory map...

    printk("[*] Initializing system calls...\n");
    init_system_call();
    printk("[*] Launching proccess...\n");
    init_proccess();
    clear_screen();
    launch();
    printk("[*] SHUTTING DOWN...\n");

    // end of kernel
}

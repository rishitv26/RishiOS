#include "lib/head.h"
#include "interrupt/inter.h"

void main(void)
{
    const char* welcome_mes = "[*] WELCOME TO RishiOS, initializing other computer processes...\0";
    int64_t val = 0x123456789ABCD; // for hex test
    init_idt();
    printk("[*] Successfully initialized IDT...");

    printk("%s\n", welcome_mes);
    printk("TEST VAL: %x\n", val);

    printk("TEST ASSERT -> ");
    ASSERT(false);
    // end of kernel
}

#include "mem.h"
#include "../lib/head.h"

static struct freemem free_mem[50];

void init_mem(void) // work on this...
{
    printk("[*] INITIALIZING MEMORY... \n");
    int32_t count = *(int32_t*)0x9000;
    uint64_t total = 0;
    struct e820 *mem_map = (struct e820*)0x9008;
    int free_c = 0;

    ASSERT(count <= 50); // there cannot be more than 50 free memory regions...

    for (int32_t i = 0; i < count; i++)
    {
        if (mem_map[i].type == 1)
        {
            free_mem[free_c].address = mem_map[i].address;
            free_mem[free_c].length = mem_map[i].length;
            total += mem_map[i].length;
            free_c++;
        }
        printk("---> %x     %uKB    %u\n", mem_map[i].address, mem_map[i].length / 1024, (uint64_t)mem_map[i].type);
    }
    printk(" -- [*] TOTAL USABLE MEMORY: %uMB\n", ((float)total)/1048576);
}

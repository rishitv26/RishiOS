#include "mem.h"
#include "../lib/head.h"

static struct freememreg free_mem[50];

void init_mem(void)
{
    int32_t count = *(int32_t*)0x9000;
    uint64_t total = 0;
    struct e280 *mem_map; // todo
}

#ifndef MEM
#define MEM

#include "stdint.h"

struct e820 // a struct to represent each block of free memory...
{
    uint64_t address;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));

struct freemem
{
    uint64_t address;
    uint64_t length;
};

void init_mem(void);

#endif // MEM
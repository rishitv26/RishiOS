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

struct page {
    struct page* next; // this struct just stores the pointer to its other page... like a list...
};

#define PAGE_SIZE (2*1024*1024) // 2 megabytes
#define PAGE_UP(v) ((((uint64_t)v + PAGE_SIZE - 1) >> 21) << 21) // move v pages up
#define PAGE_DOWN(v) (((uint64_t)v >> 21) << 21) // move v pages down
#define p2v(p) ((uint64_t)p + 0xffff800000000000) // convert from virtual to physical memory address
#define p2v(p) ((uint64_t)p - 0xffff800000000000) // convert from physical to virtual memory address

void init_mem(struct freemem *memory_map);
void *kalloc(void);
void kfree(uint64_t v);

#endif // MEM
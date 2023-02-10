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

typedef uint64_t PDE; // special types for our page conversion tables...
typedef PDE* PD;
typedef PD* PDPTR;

#define PTE_P 1 // used to define some page attributes...
#define PTE_W 2
#define PTE_U 4
#define PTE_ENTRY 0x80
#define KERNEL_BASE 0xffff800000000000

#define PAGE_SIZE (2*1024*1024) // 2 megabytes
#define PAGE_UP(v) ((((uint64_t)v + PAGE_SIZE - 1) >> 21) << 21) // move v pages up
#define PAGE_DOWN(v) (((uint64_t)v >> 21) << 21) // move v pages down
#define p2v(p) ((uint64_t)(p) + KERNEL_BASE) // convert from virtual to physical memory address
#define v2p(v) ((uint64_t)(v) - KERNEL_BASE) // convert from physical to virtual memory address
#define PDE_ADDR(p) (((uint64_t)p >> 12) << 12) // conversion table conversions...
#define PTE_ADDR(p) (((uint64_t)p >> 21) << 21) // conversion table conversions...

void init_mem(struct freemem *memory_map);
void *kalloc(void);
void kfree(uint64_t v);

void init_kvm(void);
void switch_vm(uint64_t map);
int map_pages(uint64_t map, uint64_t v, uint64_t e, uint64_t pa, uint32_t attribute);
void load_cr3(uint64_t map);

#endif // MEM
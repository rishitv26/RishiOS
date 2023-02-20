#include "mem.h"
#include "../lib/head.h"

struct freemem free_mem[50]; // the free memory...

extern char end; // defined in linker script...
static void free_region(uint64_t s, uint64_t e);
static struct page free_memory; // our first page 'node'
static uint64_t memory_end;
uint64_t page_map;

void init_mem(struct freemem *memory_map) // work on this...
{
    printk("\n[*] INITIALIZING MEMORY... (memory type 1 is usable memory for RishiOS)\n");
    int32_t count = *(int32_t*)0x9000;
    uint64_t total = 0;
    struct e820 *mem_map = (struct e820*)0x9008;
    int free_c = 0;

    ASSERT(count <= 50); // there cannot be more than 50 free memory regions...

    for (int32_t i = 0; i < count; i++)
    {
        free_mem[free_c].address = mem_map[i].address;
        free_mem[free_c].length = mem_map[i].length;
        memory_map[free_c].address = mem_map[i].address;
        memory_map[free_c].length = mem_map[i].length;
        if (mem_map[i].type == 1) // mem is usable...
        {
            total += mem_map[i].length;
            free_c++;
        }
        printk("---> %x     %uKB    %u\n", mem_map[i].address, mem_map[i].length / 1024, (uint64_t)mem_map[i].type);
    }
    printk("TOTAL FREE MOMERY -> %uMB\n[*] setting up paging...\n\n", total / 1048576);
    for (int i = 0; i < free_c; i++) // split free mem into pages of 2mb
    {
        uint64_t vstart = p2v(free_mem[i].address);
        uint64_t vend = vstart + free_mem[i].length;

        if (vstart > (uint64_t)&end)
        {
            free_region(vstart, vend);
        } 
        else if (vend > (uint64_t)&end)
        {
            free_region((uint64_t)&end, vend);
        }
    }

    memory_end = (uint64_t)free_memory.next + PAGE_SIZE; // yay, inited the pages, YIPI YAHOI!

    init_kvm(); // this is giving me the error... please help
    printk("[*] Successfully initialized memory system...\n");
}

static void free_region(uint64_t s, uint64_t e)
{
    for (uint64_t start = PAGE_UP(s); start + PAGE_SIZE <= e; start += PAGE_SIZE) // divide region into usable pages...
    {
        if (s + PAGE_SIZE <= 0xffff800040000000)
        {
            kfree(start);  
        }
    }
}

void kfree(uint64_t v)
{
    ASSERT(v % PAGE_SIZE == 0); // check if v in pagesize*n...
    ASSERT(v >= (uint64_t)&end); // it is valid memory...
    ASSERT(v + PAGE_SIZE <= 0xffff800040000000); // it is valid memory...

    struct page *page_address = (struct page*)v;
    page_address->next = free_memory.next;
    free_memory.next = page_address; // add more pages to our list!
}

void * kalloc(void) // get needed page
{
    struct page *page_addr = free_memory.next;
    if (page_addr != NULL)
    {
        ASSERT((uint64_t)page_addr % PAGE_SIZE == 0);
        ASSERT((uint64_t)page_addr >= (uint64_t)&end);
        ASSERT((uint64_t)page_addr + PAGE_SIZE <= 0xffff800040000000);

        free_memory.next = page_addr->next;
    }

    return (void*)page_addr; // return location of the page at the end...
}

static PDPTR find_pml4t_entry(uint64_t map, uint64_t v, int alloc, uint32_t attribute)
{
    PDPTR *map_entry = (PDPTR*)map;
    PDPTR pdptr = NULL;
    unsigned int index = (v >> 39) & 0x1FF;

    if ((uint64_t)map_entry[index] & PTE_P) {
        pdptr = (PDPTR)p2v(PDE_ADDR(map_entry[index]));       
    } 
    else if (alloc == 1) {
        pdptr = (PDPTR)kalloc();          
        if (pdptr != NULL) {     
            memset(pdptr, 0, PAGE_SIZE);     
            map_entry[index] = (PDPTR)(v2p(pdptr) | attribute);           
        }
    } 

    return pdptr;    
}

static PD find_pdpt_entry(uint64_t map, uint64_t v, int alloc, uint32_t attribute)
{
    PDPTR pdptr = NULL;
    PD pd = NULL;
    unsigned int index = (v >> 30) & 0x1FF;

    pdptr = find_pml4t_entry(map, v, alloc, attribute);
    if (pdptr == NULL)
        return NULL;
       
    if ((uint64_t)pdptr[index] & PTE_P) {      
        pd = (PD)p2v(PDE_ADDR(pdptr[index]));      
    }
    else if (alloc == 1) {
        pd = (PD)kalloc();  
        if (pd != NULL) {    
            memset(pd, 0, PAGE_SIZE);       
            pdptr[index] = (PD)(v2p(pd) | attribute);
        }
    } 

    return pd;
}

enum bool map_pages(uint64_t map, uint64_t v, uint64_t e, uint64_t pa, uint32_t attribute) // map the pages into a convenient map...
{
    uint64_t vstart = PAGE_DOWN(v);
    uint64_t vend = PAGE_UP(e);
    PD pd = NULL;
    unsigned int index;

    ASSERT(v < e);
    ASSERT(pa % PAGE_SIZE == 0);
    ASSERT(pa + vend - vstart <= 1024*1024*1024);

    do {
        pd = find_pdpt_entry(map, vstart, 1, attribute);    
        if (pd == NULL) {
            return false;
        }

        index = (vstart >> 21) & 0x1FF;
        ASSERT(((uint64_t)pd[index] & PTE_P) == 0);

        pd[index] = (PDE)(pa | attribute | PTE_ENTRY);

        vstart += PAGE_SIZE;
        pa += PAGE_SIZE;
    } while (vstart + PAGE_SIZE <= vend);
  
    return true;
}

void switch_vm(uint64_t map)
{
    load_cr3(v2p(map));   
}

static void setup_kvm(void) // setup the new memory management system...
{
    page_map = (uint64_t)kalloc();
    ASSERT(page_map != 0);

    memset((void*)page_map, 0, PAGE_SIZE);        
    enum bool status = map_pages(page_map, KERNEL_BASE, memory_end, v2p(KERNEL_BASE), PTE_P|PTE_W);
    ASSERT(status == true);
}

void init_kvm(void) // init function for the management system
{
    setup_kvm();
    switch_vm(page_map);
    printk("memory manager is working now\n");
}
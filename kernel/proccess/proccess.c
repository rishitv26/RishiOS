#include "proccess.h"
#include "../interrupt/inter.h"
#include "../mem/mem.h"
#include "../lib/head.h"

extern struct TSS tss; // the global TSS struct
static struct Proccess proccess_table[NUM_PROC]; // table containing all proccesses
static int pid_num = 1;
void first_proccess(void); // test function for the proccess

static void set_tss(struct Proccess *proc)
{
    tss.rsp0 = proc->stack + STACK_SIZE;
} // setup the tss for usage.

static struct Proccess* find_unused_proccess(void)
{
    struct Proccess *proccess = NULL;
    for (int i = 0; i < NUM_PROC; i++)
    {
        if (proccess_table[i].state == PROC_UNUSED)
        {
            proccess = &proccess_table[i];
            break;
        }
    }

    return proccess;
} // looks through the proccess table and creates a new proccess if there is space.

static void set_proccess_entry(struct Proccess *proc)
{
    uint64_t stack_top;

    proc->state = PROC_INIT;
    proc->pid = pid_num++;

    proc->stack = (uint64_t)kalloc();
    ASSERT(proc->stack != 0);

    memset((void*)proc->stack, 0, PAGE_SIZE);   
    stack_top = proc->stack + STACK_SIZE;

    proc->tf = (struct TrapFrame*)(stack_top - sizeof(struct TrapFrame)); 
    proc->tf->cs = 0x10|3;
    proc->tf->rip = 0x400000;
    proc->tf->ss = 0x18|3;
    proc->tf->rsp = 0x400000 + PAGE_SIZE;
    proc->tf->rflags = 0x202;
    
    proc->page_map = setup_kvm();
    ASSERT(proc->page_map != 0);
    ASSERT(setup_uvm(proc->page_map, (uint64_t)first_proccess, PAGE_SIZE));
} // set a proccess up so that it is ready to be launched.

void init_proccess(void)
{
    struct Proccess *proc = find_unused_proccess();
    ASSERT(proc == &proccess_table[0]);

    set_proccess_entry(proc);
} // initialize's the first proccess by spawning the first one.

void launch(void)
{
    set_tss(&proccess_table[0]);
    switch_vm(proccess_table[0].page_map);
    pstart(proccess_table[0].tf);
} // launch the first proccess once its ready

// our first proccess that will spawn is usermode:
void first_proccess(void)
{
    char *p = (char*)0xffff800000200020;
    *p = 1;
}

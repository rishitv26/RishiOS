#include "stdbool.h"
#include "proccess.h"
#include "../interrupt/inter.h"
#include "../mem/mem.h"
#include "../lib/head.h"

extern struct TSS tss; // the global TSS struct
static struct Proccess proccess_table[NUM_PROC]; // table containing all proccesses
static struct ProccessControl pc;
static int pid_num = 1;

static struct ProccessControl* get_pc() {
    return &pc;
}

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

static void set_proccess_entry(struct Proccess *proc, uint64_t addr)
{
    uint64_t stack_top;

    proc->state = PROC_INIT;
    proc->pid = pid_num++;

    proc->stack = (uint64_t)kalloc();
    ASSERT(proc->stack != 0);

    memset((void*)proc->stack, 0, PAGE_SIZE);   
    stack_top = proc->stack + STACK_SIZE;

    proc->context = stack_top - sizeof(struct TrapFrame) - 7*8;
    *(uint64_t*)(proc->context + 6*8) = (uint64_t)TrapReturn;

    proc->tf = (struct TrapFrame*)(stack_top - sizeof(struct TrapFrame)); 
    proc->tf->cs = 0x10|3;
    proc->tf->rip = 0x400000;
    proc->tf->ss = 0x18|3;
    proc->tf->rsp = 0x400000 + PAGE_SIZE;
    proc->tf->rflags = 0x202;
    
    proc->page_map = setup_kvm();
    ASSERT(proc->page_map != 0);
    ASSERT(setup_uvm(proc->page_map, (uint64_t)p2v(addr), 5120));
    proc->state = PROC_READY;
} // set a proccess up so that it is ready to be launched.

void init_proccess(void)
{
    struct ProccessControl *proccess_control;
    struct Proccess *proc;
    struct HeadList *list;
    uint64_t addr[] = {0x20000, 0x30000, 0x40000}; // all programs (and their addresses)

    proccess_control = get_pc();
    list = &proccess_control->ready_list;

    for (int i = 0; i < 3; ++i) {
        proc = find_unused_proccess();
        set_proccess_entry(proc, addr[i]);
        append_list_tail(list, (struct List*)proc);
    }
} // initialize's the first proccess by spawning the first one.

void launch(void)
{
    struct ProccessControl *proccess_control;
    struct Proccess *proccess;

    proccess_control = get_pc();
    proccess = (struct Proccess*)remove_list_head(&proccess_control->ready_list);
    proccess->state = PROC_RUNNING;
    proccess_control->current_proccess = proccess;

    set_tss(proccess);
    switch_vm(proccess->page_map);
    pstart(proccess->tf);
} // launch the first proccess once its ready

static void switch_proccess(struct Proccess* prev, struct Proccess* current) {
    set_tss(current);
    switch_vm(current->page_map);
    swap(&prev->context, current->context);
}

static void schedule(void) {
    struct Proccess *prev_proc;
    struct Proccess *current_proc;
    struct ProccessControl *proccess_control;
    struct HeadList* list;

    proccess_control = get_pc();
    prev_proc = proccess_control->current_proccess;
    list = &proccess_control->ready_list;
    ASSERT(!is_list_empty(list));

    current_proc = (struct Proccess*)remove_list_head(list);
    current_proc->state = PROC_RUNNING;
    proccess_control->current_proccess = current_proc;

    switch_proccess(prev_proc, current_proc);
}

void yield(void) {
    struct ProccessControl *proccess_controll;
    struct Proccess *proccess;
    struct HeadList *list;

    proccess_controll = get_pc();
    list = &proccess_controll->ready_list;

    if (is_list_empty(list)) {
        return;
    }
    
    proccess = proccess_controll->current_proccess;
    proccess->state = PROC_READY;
    append_list_tail(list, (struct List*)proccess);
    schedule();
}

// puts a proccess into sleep mode, by putting in wait list and not readylist
void sleep(int wait) {
    struct ProccessControl *proccess_control;
    struct Proccess *proccess;

    proccess_control = get_pc();
    proccess = proccess_control->current_proccess;
    proccess->state = PROC_SLEEP;
    proccess->wait = wait;

    append_list_tail(&proccess_control->wait_list, (struct List*)proccess);
    schedule();
}

void wake_up(int wait) {
    struct ProccessControl *proccess_control;
    struct Proccess *proccess;
    struct HeadList *ready_list;
    struct HeadList *wait_list;

    proccess_control = get_pc();
    ready_list = &proccess_control->ready_list;
    wait_list = &proccess_control->wait_list;
    proccess = (struct Proccess*)remove_list(wait_list, wait);

    while (proccess != NULL) {
        proccess->state = PROC_READY;
        append_list_tail(ready_list, (struct List*)proccess);
        proccess = (struct Proccess*)remove_list(wait_list, wait);
    }
}

void exit(void) {
    struct ProccessControl *proccess_controll;
    struct Proccess *proccess;
    struct HeadList *list;

    proccess_controll = get_pc();
    proccess = proccess_controll->current_proccess;
    proccess->state = PROC_KILLED;

    list = &proccess_controll->kill_list;
    append_list_tail(list, (struct List*)proccess);

    wake_up(1);
    schedule();
}

void cleanup(void) {
    struct ProccessControl *proccess_controll;
    struct Proccess *proccess;
    struct HeadList *list;

    proccess_controll = get_pc();
    list = &proccess_controll->kill_list;

    while (true) {
        if (!is_list_empty(list)) { // if there is stuff in killed list, delete it and save resources.
            proccess = (struct Proccess*)remove_list_head(list);
            ASSERT(proccess->state == PROC_KILLED);

            // actuall cleanup:
            kfree(proccess->stack);
            free_vm(proccess->page_map);
            memset(proccess, 0, sizeof(struct Proccess));
        } else { // wait otherwise.
            sleep(1);
        }
    }
}

// ============================================================================================== proccess linked list stuff:

void append_list_tail(struct HeadList* list, struct List* item) {
    item->next = NULL;

    if (is_list_empty(list)) {
        list->next = item;
        list->tail = item;
    } else {
        list->tail->next = item;
        list->tail = item;
    }
}

struct List* remove_list_head(struct HeadList *list) {
    struct List *item;

    if (is_list_empty(list)) {
        return NULL;
    }

    item = list->next;
    list->next = item->next;

    if (list->next == NULL) {
        list->tail = NULL;
    }

    return item;
}

bool is_list_empty(struct HeadList *list) {
    return list->next == NULL;
}

struct List* remove_list(struct HeadList* list, int wait) {
    struct List *current = list->next;
    struct List *prev = (struct List*)list;
    struct List *item = NULL;

    while (current != NULL) {
        // if found task with same wait, bring it back:
        if (((struct Proccess*)current)->wait == wait) {
            prev->next = current->next;
            item = current;

            if (list->next == NULL) {
                list->tail = NULL;
            } else if (current->next == NULL) {
                list->tail = prev;
            }
            break;
        }

        prev = current;
        current = current->next;
    }

    return item;
}

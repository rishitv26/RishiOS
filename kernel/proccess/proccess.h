#ifndef PROCCESS_H
#define PROCCESS_H

#include "../interrupt/inter.h"
#include "stdbool.h"

struct List {
    struct List *next;
};
struct HeadList {
    struct List* next;
    struct List* tail;
};
void append_list_tail(struct HeadList* list, struct List* item);
struct List* remove_list_head(struct HeadList *list); 
bool is_list_empty(struct HeadList *list);

// the proccess properties:
struct Proccess {
    struct List *next;
    int pid;
    int state;
    int wait;
    uint64_t context;
    uint64_t page_map;
    uint64_t stack;
    struct TrapFrame *tf;
};

// used for setting up stack pointer for a proccess
struct TSS {
    uint32_t res0;

    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t res1;

    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;

    uint64_t res2;
    uint16_t res3;
    uint16_t iopb;
} __attribute__((packed));

struct ProccessControl
{
    struct Proccess *current_proccess;
    struct HeadList ready_list;
    struct HeadList wait_list;
    struct HeadList kill_list;    
};


#define STACK_SIZE (2*1024*1024)
#define NUM_PROC 10 // how many proccesses can be there in our system.

#define PROC_UNUSED 0
#define PROC_INIT 1
#define PROC_READY 2
#define PROC_RUNNING 3
#define PROC_SLEEP 4
#define PROC_KILLED 5

void init_proccess(void); // initialize proccess
void launch(void); // launch a proccess
void pstart(struct TrapFrame *tf); // start initailization
void yield(void); // gives up CPU resources for new proccess.
void swap(uint64_t *prev, uint64_t next); // swaps proccesses
struct List* remove_list(struct HeadList* list, int wait);

void sleep(int wait);
void wake_up(int wait);
void exit(void);
void cleanup(void);

#endif // PROCCESS_H
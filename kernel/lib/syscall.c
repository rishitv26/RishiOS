#include "head.h"
#include "stddef.h"
#include "syscall.h"
#include "../proccess/proccess.h"
#include "../interrupt/inter.h"

static SYSTEMCALL system_calls[10]; // Array of system functions...

static int sys_write(int64_t *argptr) {
    write_screen((char*)argptr[0], (int)argptr[1], 0xa /*color...*/); // print suff in USER mode
    return (int)argptr[1];
}

static int sys_sleep(int64_t *argptr) {
    uint64_t old_ticks;
    uint64_t ticks;
    uint64_t sleep_ticks = argptr[0];

    ticks = get_ticks();
    old_ticks = ticks;

    while (ticks - old_ticks < sleep_ticks) {
        sleep(-1); // -1 represents that we are waiting for a tick to pass.
        ticks = get_ticks();
    }

    return 0;
}

void init_system_call(void) {
    system_calls[0] = sys_write;
    system_calls[1] = sys_sleep;
}

void system_call(struct TrapFrame *tf) { // finds appropriate system call...
    int64_t i = tf->rax;
    int64_t param_count = tf->rdi;
    int64_t *argptr = (int64_t*)tf->rsi;

    if (param_count < 0 || i != 0) {
        tf->rax = -1;
        return;
    }

    ASSERT(system_calls[i] != NULL);
    tf->rax = system_calls[i](argptr);
}
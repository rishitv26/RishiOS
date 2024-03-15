#include "../lib/head.h"
#include "stddef.h"
#include "syscall.h"
#include "../proccess/proccess.h"
#include "../interrupt/inter.h"
#include "../keyboard/keyboard.h"

static char color = 0xa;

#define SYSTEM_CALL_NUM 8

static SYSTEMCALL system_calls[SYSTEM_CALL_NUM]; // Array of system functions...

static int sys_write(int64_t *argptr) {
    write_screen((char*)argptr[0], (int)argptr[1], color /*color...*/); // print suff in USER mode
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

static int sys_exit(int64_t *argptr) {
    exit();
    return 0;
}

static int sys_cleanup(int64_t *argptr) {
    cleanup();
    return 0;
}

static int sys_clear(int64_t *argptr) {
    clear_screen();
    return 0;
}

static int sys_keyboard_read(int64_t *argptr) {
    return read_key_buffer();
}

static int sys_get_total_memory(int64_t *argptr) {
    return get_total_memory();
}

static int sys_set_color(int64_t *argptr) {
    color = (char)argptr[0];
    return color;
}

void init_system_call(void) {
    system_calls[0] = sys_write;
    system_calls[1] = sys_sleep;
    system_calls[2] = sys_exit;
    system_calls[3] = sys_cleanup;
    system_calls[4] = sys_clear;
    system_calls[5] = sys_keyboard_read;
    system_calls[6] = sys_get_total_memory;
    system_calls[7] = sys_set_color;
}

void system_call(struct TrapFrame *tf) { // finds appropriate system call...
    int64_t i = tf->rax;
    int64_t param_count = tf->rdi;
    int64_t *argptr = (int64_t*)tf->rsi;

    if (param_count < 0 || i > SYSTEM_CALL_NUM-1 || i < 0) {
        tf->rax = -1;
        return;
    }

    ASSERT(system_calls[i] != NULL);
    tf->rax = system_calls[i](argptr);
}

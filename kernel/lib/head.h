#ifndef HEAD
#define HEAD

#include "stdint.h"
#include "stdarg.h"

static const unsigned int SCREEN_WIDTH  = 160;
static const unsigned int SCREEN_HEIGHT = 25;
static char* VID_MEM = (char*)0xb8000;
static enum bool {
    true = 1,
    false = 0
};
#define NULL 0

// debug functions:
#define ASSERT(e) if (!(e)) error_check(__FILE__, __LINE__);
void error_check(char *file, uint64_t line);

// basic memory functions:
void memset(void *buffer, char val, int size); // set memory to specific byte
void memmove(void *dst, void *src, int size); // move memory into a destination
void memcpy(void *dst, void *src, int size); // copy memory to destination
int memcmp(void *src1, void *src2, int size); // compare memory locations to eachother
int strlen(void *arr); // return size of the provided pointer

// send a byte to a specific port.
static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}
// get data from a port.
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}
// wait for a small amount of time.
static inline void io_wait(void)
{
    outb(0x80, 0);
}
// read a val at different memory location at different segment.
static inline uint32_t farpeekl(uint16_t sel, void* off)
{
    uint32_t ret;
    asm ( "push %%fs\n\t"
          "mov  %1, %%fs\n\t"
          "mov  %%fs:(%2), %0\n\t"
          "pop  %%fs"
          : "=r"(ret) : "g"(sel), "r"(off) );
    return ret;
}
// check if interupts are enabled.
static inline bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}
// validate CPU.
static inline void cpuid(int code, uint32_t* a, uint32_t* d)
{
    asm volatile ( "cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx" );
}
// time since reset.
uint64_t rdtsc(void);

// basic io instructions:
struct ScreenBuffer
{
    char* buffer;
    int column;
    int row;
};
int printk(const char* format, ...);
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void update_cursor(int x, int y);

#endif // HEAD
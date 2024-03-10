#ifndef HEAD
#define HEAD

#include "stdint.h"
#include "stdarg.h"
#include "stdbool.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 25
static char* VID_MEM = (char*)0xb8000;
#define NULL 0

// assembly specific functions:
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

// debug functions:
#define ASSERT(e) if (!(e)) error_check(__FILE__, __LINE__);
void error_check(char *file, uint64_t line);

// basic memory functions:
void memset(void *buffer, char val, int size); // set memory to specific byte
void memmove(void *dst, void *src, int size); // move memory into a destination
void memcpy(void *dst, void *src, int size); // copy memory to destination
int memcmp(void *src1, void *src2, int size); // compare memory locations to eachother
int strlen(void *arr); // return size of the provided pointer

// basic io instructions:
struct ScreenBuffer
{
    char* buffer;
    int column;
    int row;
};
struct ScreenBuffer* get_buffer();
// used to print by KERNEL and bellow
int printk(const char* format, ...);
int printk_color(const char *format, char color, ...);
// used to print by USER mode.
void write_screen(const char *buffer, int size, char color);
void clear_screen();

#endif // HEAD
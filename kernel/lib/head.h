#ifndef HEAD
#define HEAD

#include "stdint.h"
#include "stdarg.h"

// debug functions:
#define ASSERT(e) if (!(e)) error_check(__FILE__, __LINE__);

void error_check(char *file, uint64_t line);

static const unsigned int SCREEN_WIDTH  = 160;
static const unsigned int SCREEN_HEIGHT = 25;
static char* VID_MEM = (char*)0xb8000;
static enum bool {
    true = 1,
    false = 0
};

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
int printk(const char* format, ...);

#endif // HEAD
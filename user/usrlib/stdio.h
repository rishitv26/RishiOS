#include "stdint.h"
#include "stdbool.h"

#ifndef _STDIO_H 
#define _STDIO_H

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 25

// io functions:
int printf(const char *format, ...);
void clearu();
unsigned char keyboard_readu(void);

// timing functions:
void sleepu(uint64_t ticks);

#endif // _STDIO_H
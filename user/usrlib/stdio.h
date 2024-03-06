#include "stdint.h"
#include "stdbool.h"

#ifndef _STDIO_H 
#define _STDIO_H

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 25

// io functions:
int printf(const char *format, ...);

// timing functions:
void sleep(uint64_t milliseconds);

#endif // _STDIO_H
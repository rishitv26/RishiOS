#ifndef MONITOR_H
#define MONITOR_H
#include "../common.h"

#define CHAR_WIDTH 80
#define CHAR_HEIGHT 25
#define VIDEO_MEM 0xb8000

unsigned int cursor_y
unsigned int cursor_x;

void monitor_put(char c);
void monitor_clear();
void monitor_write(char *c);

#endif // MONITOR_H
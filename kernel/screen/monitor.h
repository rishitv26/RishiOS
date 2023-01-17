#ifndef MONITOR_H
#define MONITOR_H
#include "../common.h"

#define CHAR_WIDTH 80
#define CHAR_HEIGHT 25
#define VIDEO_MEM 0xb8000

unsigned int cursor_y
unsigned int cursor_x;

void print_char(char c, u8int backColour, u8int foreColour);
void clear();
void print(char *c, u8int backColour, u8int foreColour);

#endif // MONITOR_H
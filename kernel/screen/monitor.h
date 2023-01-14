#ifndef MONITOR_H
#define MONITOR_H
#include "../common.h"

#define CHAR_WIDTH 80
#define CHAR_HEIGHT 25
#define VIDEO_MEM 0xb8000

const unsigned int return_cursor_y();
const unsigned int return_cursor_x();

void print_char(char c, u8int backColour, u8int foreColour);
void clear();
void print(char *c, u8int backColour, u8int foreColour);
char* itoa(int value, int base);

#endif // MONITOR_H
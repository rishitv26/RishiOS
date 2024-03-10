#ifndef _KEYBOARD_RISHIOS_H
#define _KEYBOARD_RISHIOS_H

#include "stdint.h"
#include "../lib/head.h"

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y);
uint16_t get_cursor_position(void);
uint16_t get_cursor_x(void);
uint16_t get_cursor_y(void);

// lets the user store a buffer of character from the terminal (used for IO)
struct KeyboardBuffer {
    char buffer[500]; // content
    int front;
    int end;
    int size; // size
};

// corresponding special keys.
#define E0_SIGN (1 << 0)
#define SHIFT (1 << 1)
#define CAPS_LOCK (1 << 2)

char read_key_buffer(void);
void keyboard_handler(void);
unsigned char in_byte(uint16_t port);

#endif // _KEYBOARD_H
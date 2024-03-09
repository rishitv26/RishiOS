#include "../lib/head.h"
#include "../proccess/proccess.h"
#include "keyboard.h"

// =========================== KEYBOARD MAPS: 
static unsigned char shift_code[256] = {
    [0x2A] = SHIFT, [0x36] = SHIFT, [0xAA] = SHIFT, [0xB6] = SHIFT
};

static unsigned char lock_code[256] = {
    [0x3A] = CAPS_LOCK
};

static char key_map[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', 0, 'q', 'w', 'e', 'r', 't', 'y', 'u',
    'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f',
    'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

static char shift_key_map[256] = {
    0, 1, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
    'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C',
    'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

static unsigned int flag;

static char keyboard_read(void) {
    unsigned char scan_code;
    char c;

    scan_code = in_byte(0x60);

    if (scan_code == 0xe0) { // invalid key
        flag |= E0_SIGN;
        return 0;
    }

    if (flag & E0_SIGN) { // key not used in the system
        flag &= ~E0_SIGN;
        return 0;
    }

    if (scan_code & 0x80) {
        flag &= ~(shift_code[scan_code]);
        return 0;
    }

    flag |= shift_code[scan_code];
    flag ^= lock_code[scan_code];

    if (flag & SHIFT) {
        c = shift_key_map[scan_code]; // use stuff from shift map since shift is on.
    } else {
        c = key_map[scan_code]; // otherwise use normal characters
    }

    if (flag & CAPS_LOCK) { // if caps lock is used, turn cases around.
        if ('a' <= c && c <= 'z') { // if lowcase make uppercase
            c -= 32;
        }
        else if ('A' <= c && c <= 'Z') {
            c += 32; // else make uppercase
        }
    }

    return c;
}

void keyboard_handler(void) {
    char str[2] = {0};
    str[0] = keyboard_read();
    // reads and prints the keyboard value on the screen. TODO
    if (str[0] > 0) {
        printk("%s", str);
    }
}
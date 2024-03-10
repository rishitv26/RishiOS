#include "../lib/head.h"
#include "../proccess/proccess.h"
#include "keyboard.h"

// CURSOR STUFF:
static char color = 0xf;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    // sets the hardware manually to change the status of the cursor:
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor() {
    // sets the hardware manually to disable status of the cursor.
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void update_cursor(int x, int y)
{
    // set corresponding color:
    struct ScreenBuffer* sb = get_buffer();
    sb->buffer[x*2 + y*SCREEN_WIDTH + 1] = color;
    // calculate int:
	uint16_t pos = y * (SCREEN_WIDTH)/2 + x;
    // communicate with hardware:
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

// to read position, y = pos / SCREEN_WIDTH && x = pos % SCREEN_WIDTH
uint16_t get_cursor_position(void)
{
    // read pos and return.
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

uint16_t get_cursor_x(void) {
    return get_cursor_position() % (SCREEN_WIDTH/2);
}

uint16_t get_cursor_y(void) {
    return get_cursor_position() / (SCREEN_WIDTH/2);
}

// for fun :)
void set_cursor_color(char c) {
    color = c;
}

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

static struct KeyboardBuffer key_buffer = {{0}, 0, 0, 500};
static unsigned int flag;

static char keyboard_read(void) {
    unsigned char scan_code;
    char ch;

    scan_code = in_byte(0x60);
    
    if (scan_code == 0xE0) {
        flag |= E0_SIGN;
        return 0;
    }

    if (flag & E0_SIGN) {
        flag &= ~E0_SIGN;
        return 0;
    }

    if (scan_code & 0x80) {
        flag &= ~(shift_code[scan_code]);
        return 0;
    }

    flag |= shift_code[scan_code];
    flag ^= lock_code[scan_code];

    // take care of shifting and caps stuff.
    if (flag & SHIFT) {
        ch = shift_key_map[scan_code];
    }
    else {
        ch = key_map[scan_code];
    }

    if (flag & CAPS_LOCK) {      
        if('a' <= ch && ch <= 'z')
            ch -= 32;
        else if('A' <= ch && ch <= 'Z')
            ch += 32;
    }

    return ch;
}

static void write_key_buffer(char c) {
    int front = key_buffer.front;
    int end = key_buffer.end;
    int size = key_buffer.size;

    if ((end + 1) % size == front) {
        return;
    }
    key_buffer.buffer[end++] = c;
    key_buffer.end = end % size;
}

char read_key_buffer(void) {
    int front = key_buffer.front;

    if (front == key_buffer.end) {
        sleep(-2);       
    }
    
    key_buffer.front = (key_buffer.front + 1) % key_buffer.size;
    return key_buffer.buffer[front];
}

void keyboard_handler(void) { // actuall handler that returns keys pressed so far.
    char ch = keyboard_read();

    if (ch > 0) {
        write_key_buffer(ch);
        wake_up(-2);
    }
}


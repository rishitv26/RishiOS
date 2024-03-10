#include "head.h"
#include "../mem/mem.h"
#include "../keyboard/keyboard.h"

int strlen(void *arr)
{
    int len = 0;
    while (((int*)arr + len)) len++;
    return len; // since it also counts the null element too
}

static struct ScreenBuffer screen_buffer = {(char*)p2v(0xb8000), 0, 0}; // init

static int udecimal_to_string(char *buffer, int position, uint64_t digits) // convert unsigned int to string...
{
    char digits_map[10] = "0123456789";
    char digits_buffer[25];
    int size = 0;

    do {
        digits_buffer[size++] = digits_map[digits % 10];
        digits /= 10;
    } while (digits != 0);

    for (int i = size - 1; i >= 0; i--) {
        buffer[position++] = digits_buffer[i];
    }

    return size;
}

static int decimal_to_string(char *buffer, int position, int64_t digits) // convert signed int to string... (same thing)
{
    int size = 0;

    if (digits < 0) {
        digits = -digits; // might give overflow message
        buffer[position++] = '-';
        size = 1;
    }

    size += udecimal_to_string(buffer, position, (uint64_t)digits);
    return size;
}

static int hex_to_string(char *buffer, int position, uint64_t digits) // convert hexadecimal to string
{
    char digits_buffer[25];
    char digits_map[16] = "0123456789ABCDEF";
    int size = 0;

    do {
        digits_buffer[size++] = digits_map[digits % 16];
        digits /= 16;
    } while (digits != 0);

    for (int i = size-1; i >= 0; i--) {
        buffer[position++] = digits_buffer[i];
    }

    buffer[position++] = 'h';

    return size+1;
}

static int read_string(char *buffer, int position, const char *string)  // copies the 2 strings...
{
    int index = 0;

    for (; string[index] != '\0'; index++) {
        buffer[position++] = string[index];
    }

    return index;
}

void write_screen(const char *buffer, int size, char color) // writes the character to the string
{
    struct ScreenBuffer *sb = &screen_buffer;
    int column = sb->column;
    int row = sb->row;

    for (int i = 0; i < size; i++) {       
        if (row >= 25) { // screen scroll
            memcpy(sb->buffer, sb->buffer + SCREEN_WIDTH , SCREEN_WIDTH * 24);
            memset(sb->buffer+ SCREEN_WIDTH * 24, 0, SCREEN_WIDTH);
            row--;
        }
        
        if (buffer[i] == '\n') {
            column = 0;
            row++;
        } else if (buffer[i] == '\b') { // handle backspace...
            if (column == 0 && row == 0) continue; // nothing to delete
            if (column == 0) {
                row--;
                column = SCREEN_WIDTH / 2;
            }

            column -= 1;
            sb->buffer[column*2 + row*SCREEN_WIDTH] = 0;
        }
        else {
            sb->buffer[column * 2 + row * SCREEN_WIDTH] = buffer[i];
            sb->buffer[column * 2 + row * SCREEN_WIDTH + 1] = color;

            column++;

            if (column >= 80) {
                column=0;
                row++;
            }
        }
    }

    sb->column = column;
    sb->row = row;
    update_cursor(column, row);
}

// format can be treated as a normal string.
// %x -> hex print
// %u -> unsigned int print
// %d -> int print
// %s -> string print (string in string)...
// print function can only print SCREEN_WIDTH / 2 * SCREEN_HEIGHT characters...
int printk(const char *format, ...) // the big print function with everything... returns total size of output
{
    int x, y = 0;
    char buffer[SCREEN_WIDTH / 2 * SCREEN_HEIGHT];
    int buffer_size = 0;
    int64_t integer = 0;
    char *string = 0;
    va_list args;

    va_start(args,format);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') { // normal string
            buffer[buffer_size++] = format[i];
        }
        else {
            switch (format[++i]) {
                case 'x': // hex
                    integer = va_arg(args, int64_t);
                    buffer_size += hex_to_string(buffer, buffer_size, (uint64_t)integer);
                    break;

                case 'u': // unsigned int
                    integer = va_arg(args, int64_t);
                    buffer_size += udecimal_to_string(buffer, buffer_size, (uint64_t)integer);
                    break;

                case 'd': // int
                    integer = va_arg(args, int64_t);
                    buffer_size += decimal_to_string(buffer, buffer_size, integer);
                    break;

                case 's': // string
                    string = va_arg(args, char*);
                    buffer_size += read_string(buffer, buffer_size, string);
                    break;

                default: // this is just a percent as a normal char
                    buffer[buffer_size++] = '%';
                    i--;
            }
        }
        x++;
        if (x % (SCREEN_WIDTH / 2) == 0) { y += 1; x = 0; }   
    }

    write_screen(buffer, buffer_size, 0xf); // 0xf is color, white
    va_end(args);

    return buffer_size;
}

int printk_color(const char *format, char color, ...) // the big print function with everything... returns total size of output
{
    int x, y = 0;
    char buffer[SCREEN_WIDTH / 2 * SCREEN_HEIGHT];
    int buffer_size = 0;
    int64_t integer = 0;
    char *string = 0;
    va_list args;

    va_start(args,format);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') { // normal string
            buffer[buffer_size++] = format[i];
        }
        else {
            switch (format[++i]) {
                case 'x': // hex
                    integer = va_arg(args, int64_t);
                    buffer_size += hex_to_string(buffer, buffer_size, (uint64_t)integer);
                    break;

                case 'u': // unsigned int
                    integer = va_arg(args, int64_t);
                    buffer_size += udecimal_to_string(buffer, buffer_size, (uint64_t)integer);
                    break;

                case 'd': // int
                    integer = va_arg(args, int64_t);
                    buffer_size += decimal_to_string(buffer, buffer_size, integer);
                    break;

                case 's': // string
                    string = va_arg(args, char*);
                    buffer_size += read_string(buffer, buffer_size, string);
                    break;

                default: // this is just a percent as a normal char
                    buffer[buffer_size++] = '%';
                    i--;
            }
        }
        x++;
        if (x % (SCREEN_WIDTH / 2) == 0) { y += 1; x = 0; }   
    }

    write_screen(buffer, buffer_size, color); // 0xf is color, white
    va_end(args);

    return buffer_size;
}

void error_check(char *file, uint64_t line)
{
    printk("\n ------------------------------------------- \n");
    printk("ERROR; ASSERTION FAILED IN FILE %s, LINE %u\n", file, line);
    printk("HALTING SYSTEM...");

    while (true);
}

void clear_screen() {
    for (int i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++) {             
       printk(" ");
    }
    struct ScreenBuffer* x = &screen_buffer;
    x->column = 0;
    x->row = 0;
    update_cursor(x->column, x->row);
}

struct ScreenBuffer* get_buffer() {
    return &screen_buffer;
}

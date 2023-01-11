#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
unsigned int cursor = 0;
unsigned int line = 0;

static void print_new_line();

extern void print_char(char c) // print at a location
{
    if (c == '\n') { print_new_line; return; }
    *((char*)VIDEO_MEMORY + 2 * cursor) = c;
    cursor += 1;
    if (!(cursor % SCREEN_WIDTH)) line += 1;
    return;
}
extern void print_char(char c, char color) // print at a location with colors
{
    if (c == '\n') { print_new_line; return; }
    *((char*)VIDEO_MEMORY + 2 * cursor) = c;
    *((char*)VIDEO_MEMORY + (2 * cursor + 1)) = c;
    cursor += 1;
    if (!(cursor % SCREEN_WIDTH)) line += 1;
    return;
}

extern void print(char str[])
{
    for (int i = 0; *(str + i) != '\0'; i++)
    {
        print_char(*(str + i));
    }
    return;
}
extern void print(char str[], char color)
{
    for (int i = 0; *(str + i) != '\0'; i++)
    {
        print_char(*(str + i), color);
    }
    return;
}

extern void println(char str[])
{
    print(str);
    print_new_line();
}
extern void println(char str[], char color)
{
    print(str, color);
    print_new_line();
}

static void print_new_line()
{
    const int past_line = line;
    while (past_line == line) print_char(' ');
    return;
}

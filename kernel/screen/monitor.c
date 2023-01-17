#include "monitor.h"

static void move_cursor()
{
   u16int cursorLocation = cursor_y * CHAR_WIDTH + cursor_x;
   outb(0x3D4, 14);                  // tell the hardware we are setting high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // send the high cursor byte.
   outb(0x3D4, 15);                  // tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // send the low cursor byte.
}

static void scroll()
{
   // get space character
   u8int attributeByte = (0 << 4) | (15 & 0x0F);
   u16int blank = 0x20 | (attributeByte << 8);
   if (cursor_y >= CHAR_HEIGHT)
   {
       // Move the current text chunk that is visible
       for (int i = 0; i < (CHAR_HEIGHT - 1) * (CHAR_WIDTH); i++)
       {
           *(VIDEO_MEM + (i)*2) = *(VIDEO_MEM + (i + CHAR_WIDTH)*2);
       }

       // The last line is set to blank.
       // 80 spaces to it.
       for (i = (CHAR_HEIGHT - 1) * (CHAR_WIDTH); i < CHAR_HEIGHT * CHAR_WIDTH; i++)
       {
           *(VIDEO_MEM + (i)*2) = blank;
       }
       // The cursor should now be on the last line.
       cursor_y = 24;
   }
}

void print_char(char c, u8int backColour, u8int foreColour)
{
   u16int attribute = (backColour << 4) | (foreColour & 0x0F) << 8;
   u16int *location;
   if (c == 0x08 && cursor_x) { cursor_x--; } // backspace
   else if (c == 0x09) { cursor_x = (cursor_x + 8) & ~(7); } // tab
   else if (c == '\r') { cursor_x = 0; } // carraige return
   else if (c == '\n') { cursor_x = 0; cursor_y++; } // new line
   else if(c >= ' ') // normal character
   {
       location = VIDEO_MEM + (cursor_y * CHAR_WIDTH + cursor_x);
       *location = c | attribute;
       cursor_x++;
   }
   // new line check:
   if (cursor_x >= CHAR_WIDTH) { cursor_x = 0; cursor_y ++; }
   scroll();
   move_cursor();
}

void clear()
{
    for (int i = 0; i < CHAR_HEIGHT * CHAR_WIDTH; i++) { *(VIDEO_MEM + i*2) = ''; }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void print(char *c, u8int backColour, u8int foreColour)
{
   int i = 0;
   while (c[i]) { print_char(c[i++], backColour, foreColour); }
}

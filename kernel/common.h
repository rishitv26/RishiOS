#ifndef COMMON_H
#define COMMON_H

// typedefs for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;
typedef        u32int  size_t;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);
void memcpy(void *dest, const void *src, size_t n);
void memset(char *ptr, int x, size_t n);
void memset(unsigned char *ptr, int x, size_t n);
void memset(int *ptr, int x, size_t n);
void memset(unsigned int *ptr, int x, size_t n);
size_t strlen(char *str);
size_t strlen(int *str);
size_t strlen(unsigned char *str);
size_t strlen(unsigned int *str);

#endif
#include "common.h"

void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
   u8int ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

u16int inw(u16int port)
{
   u16int ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

void memcpy(void *dest, const void *src, size_t n)
{
   // implement
}

void memset(int *ptr, int x, size_t n)
{
   for (int i = 0; i < n; i++) { *ptr = x; }
}
void memset(char *ptr, int x, size_t n)
{
   for (int i = 0; i < n; i++) { *ptr = x; }
}
void memset(unsigned char *ptr, int x, size_t n)
{
   for (int i = 0; i < n; i++) { *ptr = x; }
}
void memset(unsigned int *ptr, int x, size_t n)
{
   for (int i = 0; i < n; i++) { *ptr = x; }
}

size_t strlen(char *str)
{
   size_t i = 0;
   while (*(str + i)) { i++; }
   return i + 1;
}
size_t strlen(int *str)
{
   size_t i = 0;
   while (*(str + i)) { i++; }
   return i + 1;
}
size_t strlen(unsigned char *str)
{
   size_t i = 0;
   while (*(str + i)) { i++; }
   return i + 1;
}
size_t strlen(unsigned int *str)
{
   size_t i = 0;
   while (*(str + i)) { i++; }
   return i + 1;
}
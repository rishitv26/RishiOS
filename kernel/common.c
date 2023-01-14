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
   for (int i = 0; i < n; i++) { *((char*)dest + i) = *((char*)src + i); }
}

void memset(void *ptr, int x, size_t n)
{
   for (int i = 0; i < n; i++) { *((int*)ptr + i) = x; }
}

size_t strlen(void *str)
{
   size_t i = 0;
   while (*((char*)str + i)) { i++; }
   return i + 1;
}

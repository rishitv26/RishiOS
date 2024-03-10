#ifndef _STDMEM_H
#define _STDMEM_H

// memory info functions:
int cmd_get_total_memoryu(void);

// memory io functions...
void memset(void *buffer, char val, int size); // set memory to specific byte
void memmove(void *dst, void *src, int size); // move memory into a destination
void memcpy(void *dst, void *src, int size); // copy memory to destination
int memcmp(void *src1, void *src2, int size); // compare memory locations to eachother

#endif // _STDMEM_H
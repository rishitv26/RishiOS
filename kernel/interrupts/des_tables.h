#ifndef DES_TABLES
#define DES_TABLES

#include "../common.h"

struct idt_entry_struct // interrupt gate
{
   u16int base_lo;             // address to jump to when interrupt is fired
   u16int sel;                 // segment selector
   u8int  always0;             
   u8int  flags;               
   u16int base_hi;             // address to jump to when interrupt is fired
} __attribute__((packed));     // they are exact sized that have been mentioned
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct // array of idt_entries as pointer
{
   u16int limit;
   u32int base;                // The address of the first element in array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// functions for what to do on each interrupt: ------------------------------
extern void int0();
extern void int1();
extern void int2();
extern void int3();
extern void int4();
extern void int5();
extern void int6();
extern void int7();
extern void int8();
extern void int9();
extern void int10();
extern void int11();
extern void int12();
extern void int13();
extern void int14();
extern void int15();
extern void int16();
extern void int17();
extern void int18();
extern void int19();
extern void int20();
extern void int21();
extern void int22();
extern void int23();
extern void int24();
extern void int25();
extern void int26();
extern void int27();
extern void int28();
extern void int29();
extern void int30();
extern void int31();
// ----------------------------------------------------------------------------

#endif // DES_TABLES
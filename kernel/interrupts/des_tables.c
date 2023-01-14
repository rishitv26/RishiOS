#include "des_tables.h"

extern void idt_flush(u32int); // give computer idt table to use:
static void idt_set_gate(u8int, u32int, u16int, u8int);
static void init_idt();

idt_entry_t idt_entries[256]; // maximum amount of interrupts
idt_ptr_t   idt_ptr; // pointer to our entries

void init_idt_tables()
{
    init_idt();
}

static void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (u32int)&idt_entries;

    idt_set_gate( 0, (u32int)int0 , 0x08, 0x8E);
    idt_set_gate( 1, (u32int)int1 , 0x08, 0x8E);
    idt_set_gate( 2, (u32int)int2 , 0x08, 0x8E);
    idt_set_gate( 3, (u32int)int3 , 0x08, 0x8E);
    idt_set_gate( 4, (u32int)int4 , 0x08, 0x8E);
    idt_set_gate( 5, (u32int)int5 , 0x08, 0x8E);
    idt_set_gate( 6, (u32int)int6 , 0x08, 0x8E);
    idt_set_gate( 7, (u32int)int7 , 0x08, 0x8E);
    idt_set_gate( 8, (u32int)int8 , 0x08, 0x8E);
    idt_set_gate( 9, (u32int)int9 , 0x08, 0x8E);
    idt_set_gate( 10, (u32int)int10 , 0x08, 0x8E);
    idt_set_gate( 11, (u32int)int11 , 0x08, 0x8E);
    idt_set_gate( 12, (u32int)int12 , 0x08, 0x8E);
    idt_set_gate( 13, (u32int)int13 , 0x08, 0x8E);
    idt_set_gate( 14, (u32int)int14 , 0x08, 0x8E);
    idt_set_gate( 15, (u32int)int15 , 0x08, 0x8E);
    idt_set_gate( 16, (u32int)int16 , 0x08, 0x8E);
    idt_set_gate( 17, (u32int)int17 , 0x08, 0x8E);
    idt_set_gate( 18, (u32int)int18 , 0x08, 0x8E);
    idt_set_gate( 19, (u32int)int19 , 0x08, 0x8E);
    idt_set_gate( 20, (u32int)int20 , 0x08, 0x8E);
    idt_set_gate( 21, (u32int)int21 , 0x08, 0x8E);
    idt_set_gate( 22, (u32int)int22 , 0x08, 0x8E);
    idt_set_gate( 23, (u32int)int23 , 0x08, 0x8E);
    idt_set_gate( 24, (u32int)int24 , 0x08, 0x8E);
    idt_set_gate( 25, (u32int)int25 , 0x08, 0x8E);
    idt_set_gate( 26, (u32int)int26 , 0x08, 0x8E);
    idt_set_gate( 27, (u32int)int27 , 0x08, 0x8E);
    idt_set_gate( 28, (u32int)int28 , 0x08, 0x8E);
    idt_set_gate( 29, (u32int)int29 , 0x08, 0x8E);
    idt_set_gate( 30, (u32int)int30 , 0x08, 0x8E);
    idt_set_gate( 31, (u32int)int31, 0x08, 0x8E);

    idt_flush((u32int)&idt_ptr);
}

static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   idt_entries[num].flags   = flags /* | 0x60 */; // to be uncommented when reaching usermode
}

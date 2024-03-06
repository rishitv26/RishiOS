#include "inter.h" 
#include "../lib/head.h"
#include "../lib/syscall.h"
#include "../proccess/proccess.h"

static struct idtptr idt_pointer; // create an idt pointer
static struct idt_entry vectors[256]; // create all of the vectors seperatly
static uint64_t ticks;

uint64_t get_ticks() {
    return ticks;
}

static void timer_handler() {
    ticks++;
    wake_up(-1);
}

static void init_idt_entry(unsigned int vectnum, struct idt_entry *entry, uint64_t addr, uint8_t attribute)
{
    entry->low = (uint16_t)addr;
    entry->selector = 8;
    entry->attr = attribute;
    entry->mid = (uint16_t)(addr>>16);
    entry->high = (uint32_t)(addr>>32);
    printk("[*] Setup Interrupt %u... Successfull!\n", vectnum);
}

void init_idt(void)
{
    init_idt_entry(0, &vectors[0],(uint64_t)vector0,0x8e); // setup all of the 'vectors' individualy...
    init_idt_entry(1, &vectors[1],(uint64_t)vector1,0x8e);
    init_idt_entry(2, &vectors[2],(uint64_t)vector2,0x8e);
    init_idt_entry(3, &vectors[3],(uint64_t)vector3,0x8e);
    init_idt_entry(4, &vectors[4],(uint64_t)vector4,0x8e);
    init_idt_entry(5, &vectors[5],(uint64_t)vector5,0x8e);
    init_idt_entry(6, &vectors[6],(uint64_t)vector6,0x8e);
    init_idt_entry(7, &vectors[7],(uint64_t)vector7,0x8e);
    init_idt_entry(8, &vectors[8],(uint64_t)vector8,0x8e);
    init_idt_entry(10, &vectors[10],(uint64_t)vector10,0x8e);
    init_idt_entry(11, &vectors[11],(uint64_t)vector11,0x8e);
    init_idt_entry(12, &vectors[12],(uint64_t)vector12,0x8e);
    init_idt_entry(13, &vectors[13],(uint64_t)vector13,0x8e);
    init_idt_entry(14, &vectors[14],(uint64_t)vector14,0x8e);
    init_idt_entry(16, &vectors[16],(uint64_t)vector16,0x8e);
    init_idt_entry(17, &vectors[17],(uint64_t)vector17,0x8e);
    init_idt_entry(18, &vectors[18],(uint64_t)vector18,0x8e);
    init_idt_entry(19, &vectors[19],(uint64_t)vector19,0x8e);
    init_idt_entry(32, &vectors[32],(uint64_t)vector32,0x8e);
    init_idt_entry(39, &vectors[39],(uint64_t)vector39,0x8e);
    init_idt_entry(0x80, &vectors[0x80], (uint64_t)sysint, 0xee); // adding the system interupt.

    idt_pointer.limit = sizeof(vectors) - 1;
    idt_pointer.addr = (uint64_t)vectors;
    load_idt(&idt_pointer); // load our comeplete idt!
    printk("[*] Successfully initialized IDT...\n");
}

void handler(struct TrapFrame *tf) // the one handler function, connecting all vectors together...
{
    unsigned char isr_value;
    switch (tf->trapno) {
        case 32:
            timer_handler();
            eoi();
            break;
            
        case 39:
            isr_value = read_isr();
            if ((isr_value&(1<<7)) != 0) {
                eoi();
            }
            break;
        
        case 0x80: // the system call...
            system_call(tf);
            break;
        default:
            printk("\n[Error at %d at ring %d] errorcode %d; virtual address of error: %x; normal address of error: %x", tf->trapno, (tf->cs & 3), tf->errorcode, read_cr2(), tf->rip); // handle errors in other rings
            while (true);
    }

    if (tf->trapno == 32) {
        yield(); // give up CPU resources to start next proccess...
    }
}
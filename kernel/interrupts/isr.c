#include "isr.h"
#include "../screen/monitor.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
   print("recieved interrupt: ");
   print(itoa(regs.int_no, 10));
   print('\n');
}
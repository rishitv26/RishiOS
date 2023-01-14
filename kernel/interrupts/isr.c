#include "isr.h"
#include "../screen/monitor.h"

void isr_handler(registers_t regs) // common interrupt service routine:
{
   print("recieved interrupt: ", 0, 15);
   print(itoa(regs.int_no, 10), 0, 15);
   print_char('\n', 0, 15);
}
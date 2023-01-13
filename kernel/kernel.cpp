#include "common.h"

struct boot
{
    
}

extern "C" int main (struct boot *boot_ptr)
{
    clear();
    print("Hello Children How Are You!!!!!!\nThis is RishitOS!");
    return 0; // exit code
}
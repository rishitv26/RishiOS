#include "common.h"

extern "C" int main (struct boot *boot_ptr)
{
    monitor_clear();
    monitor_write("Hello Children How Are You!!!!!!\nThis is RishitOS!");
    return 0; // exit code
}
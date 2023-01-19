#include "sys/sys.h"

extern void shutdown();

extern int main()
{
    shutdown(); // our Kernel shut down...
    return 0; // shutdown complete
}

extern void shutdown()
{

}

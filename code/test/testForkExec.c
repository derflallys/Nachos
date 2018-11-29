#include "syscall.h"

main()
{
    PutChar('h');
    ForkExec("../test/putchar");
    ForkExec("../test/putchar");
    while(1);
}
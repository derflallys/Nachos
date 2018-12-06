#ifdef CHANGED
#include "syscall.h"

int main() {
    
    // ForkExec("../test/putchar");
    // ForkExec("../test/putchar");
    // ForkExec("../test/putchar");
    // ForkExec("../test/putchar");

    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");

    return 0;
}

#endif // CHANGED

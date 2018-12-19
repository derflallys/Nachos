#ifdef CHANGED
#include "syscall.h"
#define THIS "aaa"
#define THAT "MAIN_DU_FORK_USERPAGES1"

const int N = 10; // Choose it large enough!

void puts(const char *s) {
    const char *p; for (p = s; *p != '\0'; p++) PutChar(*p);
}

void f(void *arg) {
    const char *s = arg;
    PutString(s);
    // int i;
    // for (i = 0; i < N; i++)
    //     puts(s);
    ThreadExit();
}

int main() {
    unsigned int i = 0;
    for(i=0; i<12;i++) {
       ThreadCreate(f, "m"+i);
    }
    f(THAT);
}
#endif // CHANGED

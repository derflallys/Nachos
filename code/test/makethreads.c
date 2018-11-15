#include "syscall.h"

void printChar(char c) {
    PutChar(c);
    //PutChar('\n');
}
void printString(char * c){
    PutString(c);
}

void printInt(int i){
    PutInt(i);
}

int main() {
    volatile int i;
    for(i=0; i<9; i++) {
            ThreadCreate(printChar, 'a'+i);
    }

    //ThreadCreate(printString, "azertyuiop");
    //ThreadCreate(printInt, 98765);

    //PutChar('m');
    //PutString("qsdfghjklm");
    //PutInt(12345);

    //while(1) {}
    return 49;
}
#include "syscall.h"

void printChar(char c) {
    //int i;
    //for (i = 0; i < 10; i++) {
        PutChar(c);
    //}
    PutChar('\n');
    ThreadExit();
}
void printString(char * c){
    PutString(c);
    ThreadExit();
}

void printInt(int i){
    PutInt(i);
    ThreadExit();
}

int main() {
    volatile int i;
    ThreadCreate(printChar, 'a');
    ThreadCreate(printChar, 'b');
    ThreadCreate(printChar, 'c');

    //ThreadCreate(printString, "azertyuiop");
    //ThreadCreate(printInt, 98765);

    //PutChar('m');
    //PutString("qsdfghjklm");
    //PutInt(12345);

    ThreadExit();
    //while(1) {}
    //return 49;
}
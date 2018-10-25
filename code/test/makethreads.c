#include "syscall.h"

void printChar(char c) {
    int i;
    for (i = 0; i < 10; i++) {
        PutChar(c + i);
    }
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
    int i;
    for(i=0; i < 10; i++) {
        ThreadCreate(printChar, 'a');
    }
    //ThreadCreate(printString, "azertyuiop");
    //ThreadCreate(printInt, 98765);

    //PutChar('m');
    //PutString("qsdfghjklm");
    //PutInt(12345);

    ThreadExit();
    //while(1) {}
    //return 49;
}
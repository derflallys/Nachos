#include "syscall.h"

int
main()
{
  int end = 0;
  while(end == 0) {
    int ch = GetChar();
    PutChar(ch); //Si la valeur est bien dans le registre alors PutChar peut l'écrire
    if(ch == 33) { //fin avec le point d'exclamation -> code ASCII 33
      end = 1;
    }
  }
  return 0;
}

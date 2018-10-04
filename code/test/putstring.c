#ifdef CHANGED
#include "syscall.h"

void print(char* c)
{
  #if 1
  PutString(c);
  #endif
}
int
main()
{
  //print("abcdefgh");

  print("abcdefghijklmnopqrstuvwxyz");
  //print("\n");
  Halt();
}

#endif // CHANGED

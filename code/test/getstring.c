
#include "syscall.h"

int
main()
{
  char s[15];
  GetString(s,15);
  PutString(s);
  return 10;
}

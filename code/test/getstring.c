#include "syscall.h"

int
main()
{
  char s[10];
  GetString(s, 10);
  PutString(s);
  return 0;
}

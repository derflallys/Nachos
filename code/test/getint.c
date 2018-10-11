#include "syscall.h"

int
main()
{
  int  s ;
  GetInt(&s);
  PutInt(s);
  return 10;
}

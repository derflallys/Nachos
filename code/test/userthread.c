#include "syscall.h"
#include <stdio.h>
void
SimpleThread (void *arg)
{
    int which = (long) arg;
    int num;

    for (num = 0; num < 10; num++)
      {
	  printf ("*** thread %d looped %d times\n", which, num);
	  currentThread->Yield ();
      }
}
int main {
  ThreadCreate(SimpleThread,47);
  return 0 ;
}

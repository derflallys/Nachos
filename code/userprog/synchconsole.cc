#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *mutexChar;
static Semaphore *mutexString;
static Semaphore *mutexInt;
static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

SynchConsole::SynchConsole(const char *in, const char *out)
{
  readAvail = new Semaphore("read avail", 0);
  writeDone = new Semaphore("write done", 0);
  mutexChar = new Semaphore("mutexChar", 1);
  mutexString = new Semaphore("mutexString", 1);
  mutexInt = new Semaphore("mutexInt", 1);
  console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, 0);
}

SynchConsole::~SynchConsole()
{
  delete console;
  delete writeDone;
  delete readAvail;
  delete mutexChar;
  delete mutexString;
  delete mutexInt;
}

void SynchConsole::SynchPutChar(int ch)
{
  mutexChar->P();
  console->PutChar(ch);
  writeDone->P();
  mutexChar->V();
}

int SynchConsole::SynchGetChar()
{
  mutexChar->P();
  readAvail->P();
  return console->GetChar();
  mutexChar->V();
}

void SynchConsole::SynchPutString(const char s[])
{
  mutexString->P();
  int i = 0 ;
  while(s[i]!= '\0') {
    SynchPutChar(s[i]);
    i++;
  }
  mutexString->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
  mutexString->P();
  int i = 0 ;
  while(i < n) {
    s[i] = SynchGetChar();
    if(s[i] == '\n' || s[i] == '\0') {
      s[i] = '\0';
      break;
    }
    i++;
  }
  mutexString->V();
}

void SynchConsole::SynchPutInt(int var)
{
  mutexInt->P();
  char buf [100];
  snprintf(buf,100,"%d \n",var);
  SynchPutString(buf);
  mutexInt->V();
}

int SynchConsole::SynchGetInt(int *var)
{
  mutexInt->P();
  char * s = (char*)malloc(sizeof(char*)*100);
  SynchGetString(s,100);
  sscanf(s,"%d",var);
  free(s);
  return *var;
  mutexInt->V();
}
#endif // CHANGED

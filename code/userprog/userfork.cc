#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "userfork.h"
#include "addrspace.h"
#include "synch.h"

Semaphore*  mutexFork = new Semaphore("mutexFork", 1);

void StartFork(void * arg) {
  currentThread->space->InitRegisters ();
  currentThread->space->RestoreState ();
  machine->Run ();
}

int Fork(const char *filename) {
  AddrSpace * space;
  OpenFile *executable = fileSystem->Open(filename);
  if (executable == NULL) {
    printf ("Unable to open file %s\n", filename);
    return -1;
  }

  try {
    space = new AddrSpace(executable);
  } catch(int code) {
    printf("Erreur à la création de l'espace d'adressage. code : %d \n", code);
    return -1;
  }
  currentThread->space = space;
  mutexFork->P();
  forkCounter++;
  mutexFork->V();

  delete executable;

  Thread * newthread = new Thread("core");
  newthread->Start(StartFork,space);
  return 0;
}

void ExitFork() {
  mutexFork->P();
  forkCounter--;
  mutexFork->V();
  if(forkCounter == 0) {
    DEBUG ('q', "HALT FORK : %s \n", currentThread->getName());
    interrupt->Halt();
  } 
  else {
    DEBUG ('q', "FIN FORK : %s \n", currentThread->getName());
    delete currentThread->space;
    currentThread->Finish();
  }
}

#endif // CHANGED

#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "userthread.h"
#include "addrspace.h"

static int cpt_threads = 1;
static void StartUserThread(void *schmurtz) {
  int i;
 
  for (i = 0; i < NumTotalRegs; i++) {
    machine->WriteRegister (i, 0);
  }

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, ((struct Schmurtz*)schmurtz)->f);
    machine->WriteRegister (4, ((struct Schmurtz*)schmurtz)->arg);
    DEBUG ('x', "PCReg :  %d\n",PCReg);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
    DEBUG ('x', "NextPCReg :  %d\n",NextPCReg);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg,currentThread->space->AllocateUserStack());
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   currentThread->space->AllocateUserStack());
    DEBUG ('x', "StackReg :  %d\n",StackReg);

    machine->Run();
}

int do_ThreadCreate(int f,int arg) {
  DEBUG ('x', "f value : %d\n", f);
  DEBUG ('x', "arg value : %d\n", arg);

  Thread *newThread = new Thread("newThread");
  struct Schmurtz *schmurtz =(Schmurtz*) malloc(sizeof(Schmurtz*));
  schmurtz->f = f;
  schmurtz->arg = arg;
 
  newThread->Start(StartUserThread,schmurtz);
  cpt_threads++;
  return 0;
}

void do_ThreadExit() {
  if(cpt_threads > 1) {
    cpt_threads--;
    currentThread->Finish();
  } else {
    interrupt->Halt();
  }
}

#endif // CHANGED

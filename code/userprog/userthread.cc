#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "userthread.h"
#include "addrspace.h"
#include "synch.h"

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

    machine->WriteRegister (RetAddrReg, ((struct Schmurtz*)schmurtz)->returnVal);
    DEBUG ('x', "RetAddrReg :  %d\n",RetAddrReg);
    DEBUG ('x', "RETURN VALUE :  %d\n",((struct Schmurtz*)schmurtz)->returnVal);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg,currentThread->space->AllocateUserStack(((struct Schmurtz*)schmurtz)->which));
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   currentThread->space->AllocateUserStack(((struct Schmurtz*)schmurtz)->which));
    DEBUG ('x', "StackReg :  %d\n",StackReg);

    machine->Run();
    free(schmurtz);
}

int do_ThreadCreate(int f,int arg, int returnVal) {
  int slot = currentThread->space->findAvailableSlot();

  if(slot != -1) {
    Thread *newThread = new Thread("newThread");
    newThread->setSlot(slot);
    struct Schmurtz *schmurtz =(Schmurtz*) malloc(sizeof(Schmurtz*));
    schmurtz->f = f;
    schmurtz->arg = arg;
    schmurtz->which = slot;
    schmurtz->returnVal = returnVal;
    newThread->Start(StartUserThread,schmurtz);
    currentThread->space->incrementThreadCounter();
  }
  else {
    return -1;
  }
  return 0;
}

void do_ThreadExit() {
  if(currentThread->getSlot() != -1) {
    currentThread->space->ClearBitmap();
  }
  currentThread->space->decrementThreadCounter();
  if(currentThread->space->getThreadCounter() <= 0 && forkCounter == 0) {
    DEBUG ('q', "MACHINE HALTING (THREAD_EXIT): %s \n", currentThread->getName());
    interrupt->Halt();
  }
  DEBUG ('q', "FIN THREAD : %s \n", currentThread->getName());
  currentThread->Finish();  
}

#endif // CHANGED

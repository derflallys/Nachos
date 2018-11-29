// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#ifdef CHANGED
#include "userthread.h"
#endif //CHANGED

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
  int pc = machine->ReadRegister (PCReg);
  machine->WriteRegister (PrevPCReg, pc);
  pc = machine->ReadRegister (NextPCReg);
  machine->WriteRegister (PCReg, pc);
  pc += 4;
  machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

#ifdef CHANGED

int copyStringFromMachine(int from, char* to, unsigned size){
  int tmp;
  unsigned i = 0;
  while(i<size) {
    machine->ReadMem(from+i, 1, &tmp);
    if((char)tmp == '\0')
    break;
    to[i] = (char)tmp;
    i++;
  }
  to[i]='\0';
  return i;
}

int copyStringToMachine(int to, char* from, unsigned size) {
  unsigned i = 0;
  int tmp;
  while (i < size) {
    //printf("From Ch : %c \n",from[i] );

    if(from[i] == '\0' || from[i] == '\n') {
      break;
    }
    tmp = (int)from[i];
    machine->WriteMem(to+i,1,tmp);
    i++;
  }
  return i;
}

void Fork(void * space)
{
   
   ((AddrSpace*)space)->InitRegisters ();
   ((AddrSpace*)space)->RestoreState ();
   machine->Run ();
}

#endif // CHANGED


void
ExceptionHandler (ExceptionType which)
{
  int type = machine->ReadRegister (2);

  switch (which)
  {
    case SyscallException:
    {
      switch (type)
      {
        case SC_Halt:
        {
          DEBUG ('s', "Shutdown, initiated by user program.\n");
          interrupt->Halt ();
          break;
        }
        #ifdef CHANGED
        case SC_Exit:
        {
          DEBUG ('s', "Exit\n");
          int r = machine->ReadRegister (4);
          printf("\nExit value return  %d \n",r );
          interrupt->Halt ();
          break;
        }
        case SC_PutChar:
        {
          DEBUG ('s', "PutChar\n");
          int r = machine->ReadRegister (4);
          synchconsole->SynchPutChar(r);
          break;
        }
        case SC_PutString:
        {
          DEBUG ('s', "PutString\n");
          char* to = (char*)malloc(sizeof(char)*MAX_STRING_SIZE);
          int from = machine->ReadRegister (4);
          int r = MAX_STRING_SIZE;
          while(r == MAX_STRING_SIZE)
          {
            r = copyStringFromMachine(from,to, MAX_STRING_SIZE);
            synchconsole->SynchPutString(to);
            from += r;
          }
          free(to);
          break;
        }
        case SC_GetChar:
        {
          DEBUG ('s', "GetChar\n");
          int ch = synchconsole->SynchGetChar();
          machine->WriteRegister (2, ch);
          break;
        }
        case SC_GetString:
        {
          DEBUG ('s', "GetString\n");
          int to = machine->ReadRegister (4);
          unsigned size = machine->ReadRegister (5);
          char* string;

          if(size < MAX_STRING_SIZE) {
            string = (char*)malloc(sizeof(char)*size);
          } else {
            string = (char*)malloc(sizeof(char)*MAX_STRING_SIZE);
          }
          unsigned i = 0;
          int bufferSize;
          do {
            if(size < MAX_STRING_SIZE){
              bufferSize = size;
            }
            else {
              bufferSize = MAX_STRING_SIZE;
            }
            synchconsole->SynchGetString(string, bufferSize);
            i = copyStringToMachine(to+i, string, bufferSize);
            size -= i;
          } while(i == MAX_STRING_SIZE);
          free(string);
          break;
        }
        case SC_PutInt:
        {
          DEBUG ('s', "PutInt\n");
          int var  = machine->ReadRegister(4);
          synchconsole->SynchPutInt(var);
          break;
        }
        case SC_GetInt:
        {
          DEBUG ('s', "GetInt\n");
          int var  = machine->ReadRegister(4);
          int valueReturn = synchconsole->SynchGetInt(&var);
          machine->WriteMem(machine->ReadRegister(4),4,valueReturn);
          break;
        }
        case SC_ThreadCreate:
        {
          DEBUG ('s', "ThreadCreate\n");
          int returnVal = machine->ReadRegister(6);
          int res = do_ThreadCreate(machine->ReadRegister(4),machine->ReadRegister(5), returnVal);
          if(res == -1) {
              printf("\nStack overflow\n");
          }
          break;
        }
        case SC_ThreadExit:
        {
          DEBUG ('s', "ThreadExit\n");
          do_ThreadExit();
          break;
        }
        case SC_ForkExec:
        {
          DEBUG ('s', "ForkExec\n");
          /*char * filename = (char*)malloc(sizeof(char)*50);
          synchconsole->SynchGetString(filename, 50);
          DEBUG ('s', "filename : %s\n",filename);
          AddrSpace * space;
          OpenFile *executable = fileSystem->Open(filename);

          if (executable == NULL)
          {
            printf ("Unable to open file %s\n", filename);
            return;
          }

          space = new AddrSpace(executable);
          currentThread->space = space;
          delete executable;

          Thread * newthread = new Thread("core");
          newthread->Start(Fork,space);*/

          break;
        }
        #endif // CHANGED

        default:
        {
          printf("Unimplemented system call %d\n", type);
          ASSERT(FALSE);
        }
      }

      // Do not forget to increment the pc before returning!
      UpdatePC ();
      break;
    }

    case PageFaultException:
    if (!type) {
      printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
      ASSERT (FALSE);
    } else {
      printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
      ASSERT (FALSE);	// For now
    }

    default:
    printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
    ASSERT (FALSE);
  }
}

// addrspace.cc
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "syscall.h"
#include "new"
#include "synch.h"

#ifdef CHANGED
static void ReadAtVirtual(OpenFile*executable, int virtualaddr, int numBytes, 
int position, TranslationEntry *pageTable, unsigned numPages) {
    TranslationEntry *oldPageTable = machine->pageTable;
    unsigned oldPageTableSize = machine->pageTableSize;

    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;        

    char* buff = new char[numBytes];
    int r = executable->ReadAt(buff, numBytes, position);
    
    if (r == -1) {
        DEBUG ('a', " Erreur ReadAt pas de caractere lu \n");
    }

    for(int i = 0; i < numBytes; i++) {
        machine->WriteMem(virtualaddr+i, 1, buff[i]);
    }
    machine->pageTable = oldPageTable;
    machine->pageTableSize = oldPageTableSize;
    delete [] buff;
}
#endif // CHANGED

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    NoffHeader noffH;
    unsigned int i, size;
    
    executable->ReadAt (&noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    /* Check that this is really a MIPS program */
    ASSERT (noffH.noffMagic == NOFFMAGIC);

    // how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStacksAreaSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    // check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory
    #ifdef CHANGED
    if (numPages > pageprovider->NumAvailPage())
	    throw int(84);

    DEBUG ('a', "Initializing address space, num pages %d, total size 0x%x\n",
	   numPages, size);

    bitmap = new BitMap(UserStacksAreaSize/THREAD_SIZE);
    bitmap->Mark(0);
    currentThread->setSlot(0);
    mutex = new Semaphore("mutexThread", 1);
    threadCounter = 1; 

    // first, set up the translation
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++) {
	  pageTable[i].physicalPage = pageprovider->GetEmptyPage();	// for now, phys page # = virtual page #
	  pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on
	  // a separate page, we could set its
	  // pages to be read-only
    }
    #endif //CHANGED

    // then, copy in the code and data segments into memory
    if (noffH.code.size > 0) {
        DEBUG ('a', "Initializing code segment, at 0x%x, size 0x%x\n",
         noffH.code.virtualAddr, noffH.code.size);
        #ifdef CHANGED
        // executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
        // 	      noffH.code.size, noffH.code.inFileAddr);
        ReadAtVirtual(executable,noffH.code.virtualAddr,
            noffH.code.size, noffH.code.inFileAddr, pageTable, numPages);     
         #endif // CHANGED
    }
    if (noffH.initData.size > 0) {
        DEBUG ('a', "Initializing data segment, at 0x%x, size 0x%x\n",
                noffH.initData.virtualAddr, noffH.initData.size);
        
        #ifdef CHANGED
        // executable->ReadAt (&(machine->mainMemory[noffH.initData.virtualAddr]),
        // 	      noffH.initData.size, noffH.initData.inFileAddr);
        ReadAtVirtual(executable,noffH.initData.virtualAddr,
            noffH.initData.size, noffH.initData.inFileAddr, pageTable, numPages);
        #endif // CHANGED
    }        

    DEBUG ('a', "Area for stacks at 0x%x, size 0x%x\n",
	   size - UserStacksAreaSize, UserStacksAreaSize);

    pageTable[0].valid = FALSE;			// Catch NULL dereference
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  // LB: Missing [] for delete
  // delete pageTable;
  
  // End of modification
  #ifdef CHANGED
  delete bitmap;
  for(unsigned int i=0; i < numPages; i++) {
      pageprovider->ReleasePage(pageTable[i].physicalPage);
  }
  delete [] pageTable;
  #endif //CHANGED
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, USER_START_ADDRESS);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}

#ifdef CHANGED
int  AddrSpace::AllocateUserStack(int slot)
{
    bitmap->Mark(slot);
    return (numPages*PageSize)-16 + (slot * -256);
}

int AddrSpace::findAvailableSlot() {
    return bitmap->Find();
}

void AddrSpace::ClearBitmap() {
    return bitmap->Clear(currentThread->getSlot());
}

BitMap* AddrSpace::getBitMap() {
    return bitmap;
}

int AddrSpace::getThreadCounter() {
    return threadCounter;
}

void AddrSpace::incrementThreadCounter() {
    mutex->P();
    threadCounter++;
    forkCounter++;
    mutex->V();
}

void AddrSpace::decrementThreadCounter() {
    mutex->P();
    threadCounter--;
    forkCounter--;
    mutex->V();
}
#endif //CHANGED

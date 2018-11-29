#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "pageprovider.h"

PageProvider* PageProvider::instance = 0; //Aucune instance à la base

PageProvider* PageProvider::GetInstance(int numPages){
  if(instance == 0) //Unicité de l'instance
    instance = new PageProvider(numPages);
  return instance;
}

PageProvider::PageProvider(int numPage) {
    bitmap = new BitMap(numPage);
}

PageProvider::~PageProvider() {
    delete bitmap;
}

int PageProvider::GetEmptyPage() {
    int slot = bitmap->Find();
    if(slot != -1) {
        bitmap->Mark(slot);
        //memset(&(machine->mainMemory[slot* PageSize]), 0, PageSize);
        memset(&slot,0,4);
    }
    return slot;
}

void PageProvider::ReleasePage(int numPage) {
    bitmap->Clear(numPage);
}

int PageProvider::NumAvailPage() {
    return bitmap->NumClear();
}

#endif //CHANGED

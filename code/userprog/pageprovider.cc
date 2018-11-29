#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "pageprovider.h"


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
       memset(&slot,0,4);
   }
   return slot;
}

void PageProvider::ReleasePage(int numPage) {
    bitmap->Clear(numPage);
}

int PageProvider::NumAvailaPage() {
    return bitmap->NumClear();
}

#endif //CHANGED

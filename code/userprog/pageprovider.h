#ifdef CHANGED
#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H
#include "bitmap.h"

class PageProvider {
    private:
        BitMap* bitmap;


    public:
        PageProvider(int numPage);
        ~PageProvider();
        int GetEmptyPage();
        void ReleasePage(int numPage);
        int NumAvailaPage();
};

#endif //PAGEPROVIDER_H
#endif //CHANGED

#ifdef CHANGED
#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "bitmap.h"

class PageProvider {
    private:
        BitMap* bitmap;
        static PageProvider* instance ;
        int numPagesProvider;
        PageProvider(int numPage);

    public:
        static PageProvider *GetInstance(int numPages);
        ~PageProvider();
        int GetEmptyPage();
        void ReleasePage(int numPage);
        int NumAvailPage();
};

#endif //PAGEPROVIDER_H
#endif //CHANGED

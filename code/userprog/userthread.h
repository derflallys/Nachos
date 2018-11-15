#ifdef CHANGED
#ifndef USERTHREAD_H

 struct Schmurtz {
  int f;
  int arg;
  int which;
  int returnVal;
};

extern int do_ThreadCreate(int f, int arg, int returnVal);
extern void do_ThreadExit();

#endif //USERTHREAD_H
#endif //CHANGED

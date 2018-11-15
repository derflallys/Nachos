#ifdef CHANGED
#ifndef USERTHREAD_H

 struct Schmurtz {
  int f;
  int arg;
  int which;
};

extern int do_ThreadCreate(int f, int arg);
extern void do_ThreadExit(int addr);

#endif //USERTHREAD_H
#endif //CHANGED

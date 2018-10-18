#ifdef CHANGED
#ifndef USERTHREAD_H

typedef struct Schmurtz {
  int f;
  int arg;
} Schmurtz;

extern int do_ThreadCreate(int f, int arg);
extern void do_ThreadExit();

#endif //USERTHREAD_H
#endif //CHANGED

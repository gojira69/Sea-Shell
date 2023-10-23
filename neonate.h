#ifndef __NEONATE_H
#define __NEONATE_H

void die(const char *s);
int keyboardCapture();
pid_t getprocessID();
void neonate(int maxCommandLength, int arguments, char *miniCommand[maxCommandLength]);

#endif
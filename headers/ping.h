#ifndef __PING_H
#define __PING_H

void ping(int maxCommandLength, char *miniCommand[maxCommandLength], int arguments);
void kill_ctrl_c();
void kill_all(int maxCommandLength, char homeDirectory[maxCommandLength]);

#endif
#ifndef __WARP_H
#define __WARP_H

void warp(int maxCommandLength, int arguments, char homeDirectory[maxCommandLength], char prevDirectory[maxCommandLength], char currDirectory[maxCommandLength], char *command[maxCommandLength]);
void modchdir(int maxCommandLength, char *path, char home[maxCommandLength]);

#endif
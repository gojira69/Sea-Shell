#ifndef __PIPE_H
#define __PIPE_H

void pipeInput(int maxCommandLength, char *command, char *input, char homeDirectory[maxCommandLength], char prevDirectory[maxCommandLength], char currDirectory[maxCommandLength], bool *bg, int *fg);

#endif
#ifndef _PEEK_H
#define _PEEK_H

void peek(int maxCommandLength, char *command[maxCommandLength], char homeDirectory[maxCommandLength], int arguments);
void list(int maxCommandLength, const char *path, bool hidden, bool all);

#endif
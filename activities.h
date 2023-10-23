#ifndef __ACTIVITIES_H
#define __ACTIVITIES_H

void add_process(int maxCommandLength, pid_t pid, char homeDirectory[maxCommandLength], char *miniCommand[maxCommandLength], int arguments, int bg);
void print_processes(int maxCommandLength, char homeDirectory[maxCommandLength]);
void update_processes(int maxCommandLength, char homeDirectory[maxCommandLength]);

#endif
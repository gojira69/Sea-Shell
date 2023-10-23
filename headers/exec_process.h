#ifndef __EXEC_PROCESS_H
#define __EXEC_PROCESS_H

#define MAX_HISTORY_SIZE 15
#define MAX_COMMAND_LENGTH 4096
#define MAX_COMMANDS 30

void exec_process(int maxCommandLength, int numCommands, char **commands, bool *bgArray, char *homeDirectory, char *prevDirectory, char *currDirectory, char *input, int *fg);
void findAndReplace(char *str, char *find, char *replace);
void execute(int maxCommandLength, char *command, bool *bg, char *homeDirectory, char *prevDirectory, char *currDirectory, char *input, int *fg);
#endif
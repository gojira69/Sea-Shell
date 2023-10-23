#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

#define MAX_HISTORY_SIZE 15
#define MAX_COMMAND_LENGTH 4096

void saveHistoryToFile(const char history[][MAX_COMMAND_LENGTH], int count, char *location);
void loadHistoryFromFile(char history[][MAX_COMMAND_LENGTH], int *count, char *location);
bool isCommandInHistory(const char history[][MAX_COMMAND_LENGTH], int count, const char *command);
void storeCommands(char *miniCommand[MAX_COMMAND_LENGTH], char *input, char history[MAX_HISTORY_SIZE][MAX_COMMAND_LENGTH], int *historyCount, char *location);

#endif
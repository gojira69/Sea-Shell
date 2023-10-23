#ifndef __MISC_H
#define __MISC_H

void findAndReplace(char *str, char *find, char *replace);
void removeFirstCharacter(int maxCommandLength, char directory[maxCommandLength]);
void removeLastSlash(int maxCommandLength, char directory[maxCommandLength]);
char *concatenateCommand(char *miniCommand[], int arguments);
void freeCommands(char **commandsList, int maxCommands);
void removeHTMLTags(char *text);

#endif
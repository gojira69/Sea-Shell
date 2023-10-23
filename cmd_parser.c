#include "headers.h"

void cmd_parser(int maxCommandLength, int maxCommands, char *input, char **commandsList, int *numCommands, bool *bgArray)
{
    input[strcspn(input, "\n")] = '\0';

    int len = strlen(input);
    char *saveptr;
    int start = 0;

    for (int i = 0; i <= len; i++)
    {
        if (input[i] == '&' || input[i] == ';' || input[i] == '\0')
        {
            int length = i - start;
            if (length > 0)
            {
                strncpy(commandsList[*numCommands], input + start, length);
                commandsList[*numCommands][length] = '\0';
                if (input[i] == '&')
                {
                    bgArray[*numCommands] = true;
                }
                (*numCommands)++;
            }
            start = i + 1;
        }
    }
}
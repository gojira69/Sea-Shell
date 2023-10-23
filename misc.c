#include "headers.h"

void findAndReplace(char *str, char *find, char *replace)
{
    char *pos = strstr(str, find);

    while (pos)
    {
        int len_find = strlen(find);
        int len_replace = strlen(replace);

        if (len_replace != len_find)
            memmove(pos + len_replace, pos + len_find, strlen(pos + len_find) + 1);

        memcpy(pos, replace, len_replace);

        pos = strstr(pos + len_replace, find);
    }
}

void removeFirstCharacter(int maxCommandLength, char directory[maxCommandLength])
{
    int len = strlen(directory);
    for (int i = 0; i < len - 1; i++)
    {
        directory[i] = directory[i + 1];
    }
    directory[len - 1] = '\0';
}

void removeLastSlash(int maxCommandLength, char directory[maxCommandLength])
{
    int k = 0;
    for (int i = strlen(directory) - 1; i >= 0; i--)
    {
        if (directory[i] == '/')
        {
            k = i;
            break;
        }
    }
    directory[k] = '\0';
}

char *concatenateCommand(char *miniCommand[], int arguments)
{
    int total_length = 0;
    for (int i = 0; i < arguments; i++)
        total_length += strlen(miniCommand[i]);

    char *buffer = (char *)malloc(sizeof(char) * (total_length + arguments - 1));

    buffer[0] = '\0';

    for (int i = 0; i < arguments; i++)
    {
        strcat(buffer, miniCommand[i]);
        if (i < arguments - 1)
            strcat(buffer, " ");
    }
    return buffer;
}

void freeCommands(char **commandsList, int maxCommands)
{
    for (int i = 0; i < maxCommands; i++)
        free(commandsList[i]);
    free(commandsList);
    commandsList = NULL;
}

void removeHTMLTags(char *text)
{
    int inside_tag = 0;
    char *source = text;
    char *destination = text;

    while (*source)
    {
        if (*source == '<')
        {
            inside_tag = 1;
        }
        else if (*source == '>')
        {
            inside_tag = 0;
        }
        else if (!inside_tag)
        {
            *destination = *source;
            destination++;
        }
        source++;
    }

    *destination = '\0';
}
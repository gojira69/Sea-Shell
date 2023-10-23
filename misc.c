#include "headers/headers.h"

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

void centerText(const char *text)
{
    const char *tput_command = "tput cols";
    FILE *pipe = popen(tput_command, "r");
    if (pipe == NULL)
    {
        perror("popen");
        exit(1);
    }

    int termWidth;
    if (fscanf(pipe, "%d", &termWidth) != 1)
    {
        perror("fscanf");
        exit(1);
    }
    pclose(pipe);

    int textWidth = strlen(text);
    if (termWidth >= textWidth)
    {
        int leftPadding = (termWidth - textWidth) / 2;
        printf("\e[%dG%s\n", leftPadding + 1, text);
    }
    else
    {
        printf("%s\n", text);
    }
}

void welcomeMessage()
{
    system("clear");
    centerText("WELCOME TO SEA-SHELL!!");
    printf("LIST OF USER COMMANDS\n");
    printf("- `activities` : Displays processes spawned by the shell which are either `running` or `stopped`.\n");
    printf("- `bg` : Continues the `stopped` process in the `background`.\n");
    printf("- `fg` : Continues the `stopped` process in the `foreground`.\n");
    printf("- `iMan` : Displays the `man` page of the command.\n");
    printf("- `neonate` : Prints the `PID` of the recently created system process every `n` seconds and stops printing when the user presses the `x` key.\n");
    printf("- `pastevents` : Prints the last 15 `unique` commands executed in the shell.\n");
    printf("- `pastevents execute <index>` : Executes the nth command in the list of pastevents\n");
    printf("- `pastevents purge` : Clears the pastevents\n");
    printf("- `peek` : Prints all the files and directories in the specified directories in lexicographic order. \n");
    printf("- `ping` : It is used to send signals to processes.\n");
    printf("- `proclore` : It is used to obtain information regarding a process. If no `PID` is given, then it prints the information of the shell.\n");
    printf("- `seek` : It returns a list of relative paths (from the target directory) of all matching files/directories. \n");
    printf("- `warp` : It changes the directory that the shell is currently in.\n");
    printf("- `I/O redirection` : I/O redirection can also be done using `> , < , >>` and `|`.\n");
}

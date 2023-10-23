#include "headers.h"

int fg = -1;
int maxCommands = 30;
int maxCommandLength = 2048;

void stop_proc()
{
    if (fg > 0)
    {
        kill(fg, SIGTSTP);
        printf("Process: %d stopped\n", fg);
        fg = -1;
        fflush(stdout);
        printf("\n");
        return;
    }
}

int main()
{
    char input[maxCommandLength];
    char **commandsList = (char **)malloc(maxCommands * sizeof(char *));
    for (int i = 0; i < maxCommands; i++)
        commandsList[i] = (char *)malloc(maxCommandLength * sizeof(char));

    char homeDirectory[maxCommandLength], prevDirectory[maxCommandLength], currDirectory[maxCommandLength];
    if (getcwd(homeDirectory, sizeof(homeDirectory)) == NULL)
    {
        printf("Error: GETCWD!\n");
        return 0;
    }

    signal(SIGINT, kill_ctrl_c);
    signal(SIGTSTP, stop_proc);

    while (1)
    {
        update_processes(maxCommandLength, homeDirectory);
        bool *bgArray = (bool *)malloc(sizeof(bool) * maxCommands);
        getcwd(prevDirectory, sizeof(prevDirectory));
        for (int i = 0; i < maxCommands; i++)
            bgArray[i] = false;

        prompt(maxCommandLength, homeDirectory);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            kill_all(maxCommandLength, homeDirectory);
            update_processes(maxCommandLength, homeDirectory);
            break;
        }

        int numCommands = 0;

        cmd_parser(maxCommandLength, maxCommands, input, commandsList, &numCommands, bgArray);

        exec_process(maxCommandLength, numCommands, commandsList, bgArray, homeDirectory, prevDirectory, currDirectory, input, &fg);

        free(bgArray);
    }

    freeCommands(commandsList, maxCommands);
    return 0;
}
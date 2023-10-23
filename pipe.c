#include "headers/headers.h"

void pipeInput(int maxCommandLength, char *command, char *input, char homeDirectory[maxCommandLength], char prevDirectory[maxCommandLength], char currDirectory[maxCommandLength], bool *bg, int *fg)
{
    char *miniCommand[maxCommandLength];
    char *token = strtok(command, "|");
    int in = 0, out = 0, app = 0;
    int count = 0, nump = -1;
    while (token != NULL)
    {
        nump++;
        miniCommand[count] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(miniCommand[count++], token);
        token = strtok(NULL, "|");
    }
    if (nump == 0)
    {
        printf("Invalid use of pipe\n");
        return;
    }

    int fd[nump][2];
    for (int i = 0; i < nump; i++)
    {
        if (pipe(fd[i]) < 0)
        {
            printf("Error: PIPE!\n");
            return;
        }
    }
    pid_t pid;
    for (int i = 0; i < count; i++)
    {
        if (strlen(miniCommand[i]) == 0)
        {
            printf("Invalid use of pipe!\n");
            return;
        }

        pid = fork();
        if (pid == 0) // child process
        {
            if (i != 0)
            {
                close(fd[i - 1][1]);
                dup2(fd[i - 1][0], 0);
                close(fd[i - 1][0]);
            }
            if (i != count - 1)
                dup2(fd[i][1], 1);
            char temp[maxCommandLength];
            strcpy(temp, miniCommand[i]);
            if (strchr(temp, '>') || strchr(temp, '<'))
                I_O_Redirection(maxCommandLength, temp);

            execute(maxCommandLength, temp, bg, homeDirectory, prevDirectory, currDirectory, input, fg);
            // restoreShell();
            exit(EXIT_SUCCESS);
        }
        else if (i != count - 1)
        {
            close(fd[i][1]);
        }
    }
    for (int i = 0; i < nump; i++)
    {
        close(fd[i][0]);
        close(fd[i][1]);
    }

    for (int i = 0; i < count; ++i)
    {
        int status;
        waitpid(pid, &status, WUNTRACED);
        // wait(NULL);
    }
    return;
}
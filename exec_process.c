#include "headers/headers.h"

#define MAX_BG 100

int bgList[MAX_BG];
char bgNames[MAX_BG][1024];
int numBG = 0;

void add_background_process(pid_t pid, char *proc_name)
{
    if (numBG < MAX_BG)
    {
        bgList[numBG] = pid;
        strcpy(bgNames[numBG], proc_name);
        numBG++;
    }
}

void handle_background_processes()
{
    int i;
    int status;
    for (i = 0; i < numBG; i++)
    {
        pid_t pid = bgList[i];
        if (waitpid(pid, &status, WNOHANG) > 0)
        {
            if (WIFEXITED(status))
            {
                int exit_status = WEXITSTATUS(status);
                if (exit_status == 0)
                {
                    printf("%s with PID: %d exited normally.\n", bgNames[i], pid);
                }
                else
                {
                    printf("%s with PID: %d exited abnormally\n", bgNames[i], pid);
                }
            }
            numBG--;
        }
    }
}

void execute(int maxCommandLength, char *command, bool *bg, char *homeDirectory, char *prevDirectory, char *currDirectory, char *input, int *fg)
{
    bool back = *bg;
    char history[MAX_HISTORY_SIZE][MAX_COMMAND_LENGTH];
    char org_cmd[maxCommandLength];
    int historyCount = 0;

    char hist_location[maxCommandLength];
    sprintf(hist_location, "%s/history.txt", homeDirectory);

    loadHistoryFromFile(history, &historyCount, hist_location);

    if (historyCount == MAX_HISTORY_SIZE)
    {
        historyCount = 0;
        saveHistoryToFile(history, historyCount, hist_location);
    }

    char *miniCommand[maxCommandLength];
    int arguments = 0;
    char *token = strtok(command, " ");
    while (token != NULL)
    {
        miniCommand[arguments++] = token;
        token = strtok(NULL, " ");
    }

    if (miniCommand[0] == NULL)
    {
        printf("Error: Invalid command!\n");
        return;
    }

    miniCommand[arguments] = NULL;
    if (!strcmp(miniCommand[0], "proclore"))
    {
        int pid;
        if (miniCommand[1] != NULL)
        {
            pid = atoi(miniCommand[1]);
            proclore(pid, maxCommandLength, homeDirectory);
        }
        else
        {
            pid = getpid();
            proclore(pid, maxCommandLength, homeDirectory);
        }
    }
    else if (!strcmp(miniCommand[0], "warp"))
    {
        warp(maxCommandLength, arguments, homeDirectory, prevDirectory, currDirectory, miniCommand);
    }
    else if (!strcmp(miniCommand[0], "peek"))
    {
        peek(maxCommandLength, miniCommand, homeDirectory, arguments);
    }
    else if (!strcmp(miniCommand[0], "seek"))
    {
        seek(maxCommandLength, arguments, miniCommand, homeDirectory);
    }
    else if (!strcmp(miniCommand[0], "pastevents"))
    {
        if (arguments == 1)
        {
            for (int i = historyCount - 1; i >= 0; i--)
            {
                printf("%d: %s\n", historyCount - i, history[i]);
            }
        }
        else if (strcmp(miniCommand[1], "purge") == 0)
        {
            historyCount = 0;
            saveHistoryToFile(history, historyCount, hist_location);
        }
        else if (strcmp(miniCommand[1], "execute") == 0 && arguments == 3)
        {
            int index = atoi(miniCommand[2]);
            if (index > 0 && index < historyCount + 1)
            {
                if (historyCount > 1 && index == historyCount + 1)
                {
                    printf("Cannot execute\n");
                }
                else
                {
                    bool *ps_bgArray = (bool *)malloc(sizeof(bool) * MAX_COMMANDS);
                    for (int i = 0; i < MAX_COMMANDS; i++)
                        ps_bgArray[i] = false;

                    int executeIndex = historyCount - index;

                    sprintf(org_cmd, "pastevents execute %d", index);

                    char ps_input[maxCommandLength];
                    strcpy(ps_input, history[executeIndex]);
                    char **commandsList = (char **)malloc(MAX_COMMANDS * sizeof(char *));
                    for (int i = 0; i < MAX_COMMANDS; i++)
                        commandsList[i] = (char *)malloc(maxCommandLength * sizeof(char));

                    int ps_commands = 0;

                    cmd_parser(maxCommandLength, MAX_COMMANDS, ps_input, commandsList, &ps_commands, ps_bgArray);

                    exec_process(maxCommandLength, ps_commands, commandsList, ps_bgArray, homeDirectory, prevDirectory, currDirectory, ps_input, fg);

                    findAndReplace(input, org_cmd, history[executeIndex]);

                    storeCommands(miniCommand, input, history, &historyCount, hist_location);

                    freeCommands(commandsList, MAX_COMMANDS);
                    free(ps_bgArray);
                }
            }
            else
            {
                printf("Invalid index.\n");
                return;
            }
        }
    }
    else if (!strcmp(miniCommand[0], "exit"))
    {
        saveHistoryToFile(history, historyCount, hist_location);
        char lastloc[maxCommandLength];
        strcpy(lastloc, homeDirectory);
        strcat(lastloc, ".prev_dir_");
        remove(lastloc);
        exit(EXIT_SUCCESS);
    }
    else if (!strcmp(miniCommand[0], "iMan"))
    {
        iMan(miniCommand[1]);
    }
    else if (!strcmp(miniCommand[0], "activities"))
    {
        print_processes(maxCommandLength, homeDirectory);
    }
    else if (!strcmp(miniCommand[0], "ping"))
    {
        ping(maxCommandLength, miniCommand, arguments);
    }
    else if (!strcmp(miniCommand[0], "fg"))
    {
        if (arguments < 2)
        {
            printf("Error: FG! Invalid syntax\n");
            return;
        }
        int procId = atoi(miniCommand[1]);
        stopTofg(procId);
    }
    else if (!strcmp(miniCommand[0], "bg"))
    {
        if (arguments < 2)
        {
            printf("Error: BG! Invalid syntax\n");
            return;
        }
        int procId = atoi(miniCommand[1]);
        bgTorunning(procId);
    }
    else if (!strcmp(miniCommand[0], "neonate"))
    {
        neonate(maxCommandLength, arguments, miniCommand);
    }
    else
    {
        pid_t newProcessID = fork();
        if (newProcessID == 0) // child
        {
            miniCommand[0][strcspn(miniCommand[0], "\n")] = '\0';
            execvp(miniCommand[0], miniCommand);
            perror("Exec error");
            exit(1);
        }
        else if (newProcessID > 0) // parent
        {
            add_process(maxCommandLength, newProcessID, homeDirectory, miniCommand, arguments, back);
            if (!back)
            {
                *fg = newProcessID;
                struct timeval start_time, end_time;
                gettimeofday(&start_time, NULL);

                int status;
                waitpid(newProcessID, &status, WUNTRACED);

                gettimeofday(&end_time, NULL);

                double execution_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

                if (execution_time > 2.0)
                    printf("%s took %d seconds to run.\n", miniCommand[0], (int)execution_time);
            }
            else
            {
                add_background_process(newProcessID, miniCommand[0]);
                printf("%s started with PID: %d in the background\n", miniCommand[0], newProcessID);
            }
        }
        else
            perror("Fork error");
    }
    storeCommands(miniCommand, input, history, &historyCount, hist_location);
}

void exec_process(int maxCommandLength, int numCommands, char **commands, bool *bgArray, char *homeDirectory, char *prevDirectory, char *currDirectory, char *input, int *fg)
{
    handle_background_processes();
    update_processes(maxCommandLength, homeDirectory);
    for (int i = 0; i < numCommands && commands[i] != NULL; i++)
    {
        I_O_Redirection(maxCommandLength, commands[i]);
        if (strchr(commands[i], '|'))
        {
            pipeInput(maxCommandLength, commands[i], input, homeDirectory, prevDirectory, currDirectory, &bgArray[i], fg);
            restoreShell();
            getcwd(currDirectory, sizeof(currDirectory));
            continue;
        }
        execute(maxCommandLength, commands[i], &bgArray[i], homeDirectory, prevDirectory, currDirectory, input, fg);
        restoreShell();
        getcwd(currDirectory, sizeof(currDirectory));
    }
    return;
}
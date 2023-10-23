#include "headers.h"

void kill_ctrl_c()
{
    printf("\n");
    return;
}

void kill_all(int maxCommandLength, char homeDirectory[maxCommandLength])
{
    char path[maxCommandLength];
    strcpy(path, homeDirectory);
    strcat(path, "/proc");
    FILE *proc = fopen(path, "r");
    if (proc == NULL)
        return;
    char buffer[maxCommandLength];

    while (fgets(buffer, sizeof(buffer), proc) != NULL)
    {
        int process_id;
        char proc_name[maxCommandLength], state[maxCommandLength], buffer2[maxCommandLength];
        int res = sscanf(buffer, "%d\t:\t'%[^']'\t-\t%s", &process_id, proc_name, state);
        if (process_id > 0)
            kill(process_id, 9);
    }
    fclose(proc);
    return;
}

void ping(int maxCommandLength, char *miniCommand[maxCommandLength], int arguments)
{
    if (arguments != 3)
    {
        printf("Invalid command for ping!\n");
        return;
    }

    int sig_num = (atoi(miniCommand[2])) % 32;
    int process_id = atoi(miniCommand[1]);

    if (process_id <= 0)
    {
        printf("Error: Invalid PID!\n");
        return;
    }

    if (kill(process_id, sig_num) != 0)
    {
        printf("Error: KILL\n");
        return;
    }
    printf("Sent signal %d to process with pid %d\n", sig_num, process_id);
}
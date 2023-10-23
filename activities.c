#include "headers/headers.h"

void add_process(int maxCommandLength, pid_t pid, char homeDirectory[maxCommandLength], char *miniCommand[maxCommandLength], int arguments, int bg)
{
    char path[maxCommandLength];
    strcpy(path, homeDirectory);
    strcat(path, "/proc");
    FILE *proc = fopen(path, "a");

    char *command = concatenateCommand(miniCommand, arguments);

    fprintf(proc, "%d\t:\t'%s'\t-\tRunning\n", pid, command);

    fclose(proc);

    free(command);
}

void update_processes(int maxCommandLength, char homeDirectory[maxCommandLength])
{
    char path[maxCommandLength];
    strcpy(path, homeDirectory);
    strcat(path, "/proc");

    FILE *proc = fopen(path, "r");
    if (proc == NULL)
        return;

    char buffer[maxCommandLength];
    char updated_buffer[maxCommandLength];

    FILE *temp = tmpfile();
    if (temp == NULL)
        return;

    while (fgets(buffer, sizeof(buffer), proc) != NULL)
    {
        int process_id;
        char proc_name[maxCommandLength], state[maxCommandLength], buffer2[maxCommandLength];
        int res = sscanf(buffer, "%d\t:\t'%[^']'\t-\t%s", &process_id, proc_name, state);

        char org_path[maxCommandLength];
        snprintf(org_path, sizeof(org_path), "/proc/%d/status", process_id);
        int p = open(org_path, O_RDONLY);
        if (p == -1)
            fprintf(temp, "\0", buffer);
        else
        {
            long long int readBytes = read(p, buffer2, sizeof(buffer2));

            char *string = strstr(buffer2, "State:");
            char status;
            sscanf(string, "State:\t%c\n", &status);

            if (status == 'T')
            {
                fprintf(temp, "%d\t:\t'%s'\t-\tStopped\n", process_id, proc_name);
                close(p);
            }
            else if (status == 'S' || status == 'R')
            {
                fprintf(temp, "%d\t:\t'%s'\t-\tRunning\n", process_id, proc_name);
                close(p);
            }
        }
    }

    fclose(proc);

    proc = fopen(path, "w");
    if (proc == NULL)
    {
        fclose(temp);
        return;
    }

    rewind(temp);
    while (fgets(updated_buffer, sizeof(updated_buffer), temp) != NULL)
        fprintf(proc, "%s", updated_buffer);

    fclose(proc);
    fclose(temp);
    return;
}

void print_processes(int maxCommandLength, char homeDirectory[maxCommandLength])
{
    char path[maxCommandLength];
    strcpy(path, homeDirectory);
    strcat(path, "/proc");
    FILE *proc = fopen(path, "r");
    char buffer[maxCommandLength];
    while (fgets(buffer, sizeof(buffer), proc) != NULL)
        printf("%s", buffer);

    fclose(proc);
}
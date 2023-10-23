#include "headers/headers.h"

void bgTorunning(int pid)
{

    if (pid <= 0)
    {
        printf("Error: Invalid PID!\n");
        return;
    }

    if (kill(pid, SIGCONT) != 0)
    {
        printf("Error: KILL!\n");
        return;
    }
    return;
}
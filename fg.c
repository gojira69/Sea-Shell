#include "headers.h"

void stopTofg(int pid)
{

    if (pid <= 0)
    {
        printf("Error: Invalid PID!\n");
        return;
    }

    int status;
    if (kill(pid, SIGCONT) == 0)
    {
        waitpid(pid, &status, WUNTRACED);
    }
    else
    {
        printf("Error: KILL!\n");
        return;
    }
    return;
}
#include "headers/headers.h"

// go to proc path1 and open the status file of the process- /proc/pid/status
// and perform a readlink on exe to get resolved path

void proclore(pid_t pid, int maxCommandLength, char homeDirectory[maxCommandLength])
{
    char path1[4096];
    char exe_path[30] = "~/a.out";
    // for finding the resolved path
    snprintf(path1, sizeof(path1), "/proc/%d/exe", pid);
    char buffer1[4096], buffer2[4096];
    bool isHome = true;
    int readFile = readlink(path1, buffer1, sizeof(buffer1));

    buffer1[readFile] = '\0';

    // printf("\n\n%s\n\n", buffer1);

    // printf("%d\n", sizeof(buffer1));

    char path2[4096];

    // for finding the virual memory
    snprintf(path2, sizeof(path2), "/proc/%d/status", pid);
    int status_fd = open(path2, O_RDONLY);
    long long int readBytes = read(status_fd, buffer2, sizeof(buffer2));
    if (readBytes == -1)
    {
        printf("Process not found with pid %d\n", pid);
        return;
    }
    char *search_string = strstr(buffer2, "VmSize:");
    long long int VMsize;
    sscanf(search_string, "VmSize: %lld kB", &VMsize);

    // for finding the process status
    char *string = strstr(buffer2, "State:");
    char status;
    sscanf(string, "State:\t%c", &status);

    printf("pid : %d\n", pid);
    printf("Process Status : %c\n", status);
    printf("Process Group : %d\n", getpgid(pid));
    printf("Virtual Memory : %d\n", VMsize);
    removeSubstring(buffer1, homeDirectory);
    printf("Executable Path : %s\n", buffer1);

    close(status_fd);
}
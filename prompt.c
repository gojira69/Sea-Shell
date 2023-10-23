#include "headers/headers.h"

#define DEFAULT "\x1b[0m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define WHITE "\x1b[37m"

void removeSubstring(char *str, const char *sub)
{
    char *match = strstr(str, sub);
    while (match != NULL)
    {
        size_t remaining_len = strlen(match + strlen(sub));

        memmove(match, match + strlen(sub), remaining_len + 1);

        match = strstr(str, sub);
    }
}

void prompt(int maxCommandLength, char homeDirectory[maxCommandLength])
{
    // Do not hardcode the prmopt
    char hostname[256];
    char username[256];
    char path[256];
    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        printf("Error: GETHOSTNAME!\n");
        return;
    }

    if (getlogin_r(username, sizeof(username)) == -1)
    {
        printf("Error: GETLOGIN!\n");
        return;
    }

    if (getcwd(path, sizeof(path)) == NULL)
    {
        printf("Error: GETCWD!\n");
        return;
    }

    removeSubstring(path, homeDirectory);

    if (strcmp(path, homeDirectory) == 0)
        printf(BLUE "<%s" DEFAULT "@%s:~>", username, hostname);
    else
        printf(BLUE "<%s@" DEFAULT "%s:%s~>", username, hostname, path);
}
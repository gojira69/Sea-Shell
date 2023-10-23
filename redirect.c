#include "headers.h"

int dest = 0, src = 0, orgdest = 0, orgsrc = 0;

void restoreShell()
{
    if (dest)
    {
        close(dest);
        dest = 0;
        dup2(orgdest, STDIN_FILENO);
        close(orgdest);
    }

    if (src)
    {
        close(src);
        src = 0;
        dup2(orgsrc, STDOUT_FILENO);
        close(orgsrc);
    }
}


void I_O_Redirection(int maxCommandLength, char *input)
{
    char temp[maxCommandLength] = {};
    char com[maxCommandLength];
    strcpy(com, input);
    int in = 0, w = 0;

    char *tok = strtok(com, " \n\t\r\v\f");
    while (tok != NULL)
    {
        if (in > 0)
        {
            in = 0;
            dest = open(tok, O_RDONLY);
            if (dest < 0)
            {
                dest = 0;
                src = 0;
                restoreShell();
                return;
            }
        }
        else if (w == 1)
        {
            w = 0;
            src = open(tok, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        else if (w == 2)
        {
            w = -1;
            src = open(tok, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else if (strcmp(tok, "<") == 0)
        {
            in = 1;
        }
        else if (strcmp(tok, ">") == 0)
        {
            w = 1;
        }
        else if (strcmp(tok, ">>") == 0)
        {
            w = 2;
        }
        else if (in == 0 && w == 0)
        {
            strcat(temp, " ");
            strcat(temp, tok);
        }

        tok = strtok(NULL, " \n\t\r\v\f");
    }
    strcpy(input, temp);
    if (dest)
    {
        orgdest = dup(STDIN_FILENO);
        dup2(dest, STDIN_FILENO);
    }

    if (src)
    {
        orgsrc = dup(STDOUT_FILENO);
        dup2(src, STDOUT_FILENO);
    }
    return;
}

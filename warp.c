#include "headers.h"

void modchdir(int maxCommandLength, char *path, char home[maxCommandLength])
{
    char prev_dir[maxCommandLength];
    if (getcwd(prev_dir, sizeof(prev_dir)) == NULL)
    {
        printf("Error: GETCWD!\n");
        return;
    }

    char lastLocation[maxCommandLength];
    strcpy(lastLocation, home);
    strcat(lastLocation, "/.prev_dir_");

    if (strcmp(path, "-") == 0)
    {
        FILE *temp_file = fopen(lastLocation, "r");
        if (temp_file == NULL)
        {
            printf("OLDPWD not set\n");
            return;
        }
        char temp[maxCommandLength];
        fgets(temp, sizeof(temp), temp_file);
        temp[strcspn(temp, "\n")] = '\0';
        if (chdir(temp) == -1)
        {
            printf("Invalid Location\n");
            return;
        }
        fclose(temp_file);
    }
    else
    {
        if (chdir(path) == -1)
        {
            printf("Invalid Location\n");
            return;
        }
    }

    FILE *prev_dir_ = fopen(lastLocation, "w");
    fprintf(prev_dir_, "%s\n", prev_dir);
    fclose(prev_dir_);
}

void warp(int maxCommandLength, int arguments, char homeDirectory[maxCommandLength], char prevDirectory[maxCommandLength], char currDirectory[maxCommandLength], char *command[maxCommandLength])
{
    if (arguments == 1)
        modchdir(maxCommandLength, homeDirectory, homeDirectory);
    else
    {
        for (int i = 1; i < arguments; i++)
        {
            if (strcmp(command[i], ".") == 0)
            {
                //. does not do anything but goes to the same directory
            }
            else if (strcmp(command[i], "..") == 0)
            {
                modchdir(maxCommandLength, command[i], homeDirectory);
            }
            else if (strcmp(command[i], "-") == 0)
            {
                modchdir(maxCommandLength, command[i], homeDirectory);
            }
            else if (command[i][0] == '~')
            {
                char newPath[maxCommandLength];
                removeFirstCharacter(maxCommandLength, command[i]);
                strcpy(newPath, homeDirectory);
                strcat(newPath, command[1]);
                modchdir(maxCommandLength, newPath, homeDirectory); // goes to the home directory of the shell
            }
            else
            {
                modchdir(maxCommandLength, command[i], homeDirectory);
            }
        }
    }
    char present_dir[maxCommandLength];
    getcwd(present_dir, sizeof(present_dir));
    if (!strcmp(present_dir, homeDirectory))
    {
        printf("/\n");
        return;
    }
    removeSubstring(present_dir, homeDirectory);
    printf("%s\n", present_dir);
    return;
}
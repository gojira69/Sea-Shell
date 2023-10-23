#include "headers.h"

#define WHITE "\x1b[0m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"

#define BUF 10000

void search_rec(int maxCommandLength, const char *target, char homeDirectory[maxCommandLength], const char *directory, int s_files, int s_dirs, int s_exe)
{
    struct dirent *file;
    DIR *direct = opendir(directory);

    if (direct == NULL)
    {
        return;
    }

    while ((file = readdir(direct)) != NULL)
    {
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
        {
            continue;
        }

        char completePath[maxCommandLength];
        snprintf(completePath, sizeof(completePath), "%s/%s", directory, file->d_name);

        struct stat file_stat;
        if (lstat(completePath, &file_stat) == -1)
        {
            printf("Error: LSTAT!\n");
            return;
        }

        if ((s_files && S_ISREG(file_stat.st_mode)) || (s_dirs && S_ISDIR(file_stat.st_mode)))
        {
            if (strcmp(file->d_name, target) == 0)
            {
                if (s_exe)
                {
                    if (S_ISDIR(file_stat.st_mode))
                    {
                        if (chdir(completePath) == -1)
                        {
                            perror("chdir");
                        }
                        return;
                    }
                    else if (S_ISREG(file_stat.st_mode))
                    {
                        FILE *cat_file = fopen(completePath, "r");
                        if (cat_file == NULL)
                        {
                            perror("fopen");
                            return;
                        }

                        char buffer[BUF];
                        while (fgets(buffer, sizeof(buffer), cat_file))
                        {
                            printf("%s", buffer);
                        }

                        fclose(cat_file);
                        return;
                    }
                }
                else
                {
                    removeSubstring(completePath, homeDirectory);
                    if (S_ISDIR(file_stat.st_mode))
                        printf(BLUE "%s\n" WHITE, completePath);
                    else
                        printf(GREEN "%s\n" WHITE, completePath);
                }
            }
        }

        if (S_ISDIR(file_stat.st_mode))
        {
            search_rec(maxCommandLength, target, homeDirectory, completePath, s_files, s_dirs, s_exe);
        }
    }
    closedir(direct);
}

void seek(int maxCommandLength, int arguments, char *command[maxCommandLength], char homeDirectory[maxCommandLength])
{
    int s_files = 0, s_dir = 0, s_exe = 0;
    const char *directory = command[arguments - 1];
    const char *target = command[arguments - 2];
    char currdir[maxCommandLength];
    if (getcwd(currdir, sizeof(currdir)) == NULL)
    {
        printf("Error: GETCWD!\n");
        return;
    }
    if (arguments > 5 || arguments < 2)
    {
        printf("Error - Incorrect Syntax of seek\n");
        return;
    }
    if (arguments == 2)
    {
        target = command[1];
        if (s_files == 0 && s_dir == 0)
        {
            s_files = 1;
            s_dir = 1;
        }
        search_rec(maxCommandLength, target, homeDirectory, currdir, s_files, s_dir, s_exe);
        return;
    }
    if (arguments >= 3)
    {
        for (int i = 0; i < arguments; i++)
        {
            if (strcmp(command[i], "-d") == 0)
            {
                s_dir = 1;
            }
            else if (strcmp(command[i], "-f") == 0)
            {
                s_files = 1;
            }
            else if (strcmp(command[i], "-ef") == 0)
            {
                s_files = 1;
                s_exe = 1;
            }
            else if (strcmp(command[i], "-fe") == 0)
            {
                s_files = 1;
                s_exe = 1;
            }
            else if (strcmp(command[i], "-de") == 0)
            {
                s_dir = 1;
                s_exe = 1;
            }
            else if (strcmp(command[i], "-ed") == 0)
            {
                s_dir = 1;
                s_exe = 1;
            }
            else if (strcmp(command[i], "-fd") == 0)
            {
                printf("Invalid Flags!\n");
                return;
            }
            else if (strcmp(command[i], "-df") == 0)
            {
                printf("Invalid Flags!\n");
                return;
            }
            else if (strcmp(command[i], "-e") == 0)
            {
                s_exe = 1;
            }
        }
        if (s_files == 1 && s_dir == 1)
        {
            printf("Invalid Flags!\n");
            return;
        }
        if (s_files == 0 && s_dir == 0)
        {
            s_files = 1;
            s_dir = 1;
        }
        search_rec(maxCommandLength, target, homeDirectory, directory, s_files, s_dir, s_exe);
    }
    else
    {
        for (int i = 0; i < arguments; i++)
        {
            if (strcmp(command[i], "-d") == 0)
            {
                s_dir = 1;
            }
            else if (strcmp(command[i], "-f") == 0)
            {
                s_files = 1;
            }
            else if (strcmp(command[i], "-ef") == 0)
            {
                s_files = 1;
                s_exe = 1;
            }
            else if (strcmp(command[i], "-fe") == 0)
            {
                s_files = 1;
                s_exe = 1;
            }
            else if (strcmp(command[i], "-de") == 0)
            {
                s_dir = 1;
                s_exe = 1;
            }
            else if (strcmp(command[i], "-ed") == 0)
            {
                s_dir = 1;
                s_exe = 1;
            }
            else if (strcmp(command[i], "-fd") == 0)
            {
                printf("Invalid Flags!\n");
                return;
            }
            else if (strcmp(command[i], "-df") == 0)
            {
                printf("Invalid Flags!\n");
                return;
            }
            else if (strcmp(command[i], "-e") == 0)
            {
                s_exe = 1;
            }
        }
        if (s_files == 1 && s_dir == 1)
        {
            printf("Invalid Flags!\n");
            return;
        }
        if (s_files == 0 && s_dir == 0)
        {
            s_files = 1;
            s_dir = 1;
        }
        search_rec(maxCommandLength, target, homeDirectory, currdir, s_files, s_dir, s_exe);
    }
}
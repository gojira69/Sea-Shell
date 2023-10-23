#include "headers/headers.h"

#define DEFAULT "\x1b[0m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define WHITE "\x1b[37m"

int sort(const struct dirent **a, const struct dirent **b)
{
    return strcoll((*a)->d_name, (*b)->d_name);
}

void list(int maxCommandLength, const char *path, bool hidden, bool all)
{
    struct dirent **fileList2;
    int fileCount = scandir(path, &fileList2, NULL, sort);

    if (fileCount == -1)
    {
        printf("Invalid Path\n");
        return;
    }

    int temp = fileCount;
    int blockSize = 0;
    int j = 0;
    while (temp)
    {
        //  if a flag is set
        //  files like . and .. will not be shown if flag is not set
        if (!hidden && (strcmp(fileList2[j]->d_name, ".") == 0 || strcmp(fileList2[j]->d_name, "..") == 0))
        {
            free(fileList2[j++]);
            temp--;
            continue;
        }
        char completePath2[maxCommandLength * 2];
        snprintf(completePath2, sizeof(completePath2), "%s/%s", path, fileList2[j]->d_name);

        struct stat fileDetails2;
        if (lstat(completePath2, &fileDetails2) == -1)
        {
            printf("Error: LSTAT! bitch\n");
            return;
        }
        blockSize += fileDetails2.st_blocks;
        temp--;
    }

    printf("%d\n", blockSize);
    free(fileList2);

    struct dirent **fileList;
    fileCount = scandir(path, &fileList, NULL, sort);
    if (fileCount == -1)
    {
        printf("Invalid Path\n");
        return;
    }

    int i = 0, k = 0;
    while (fileCount)
    {
        // if a flag is set
        //  files like . and .. will not be shown if flag is not set
        if (!hidden && (strcmp(fileList[i]->d_name, ".") == 0 || strcmp(fileList[i]->d_name, "..") == 0))
        {
            free(fileList[i++]);
            fileCount--;
            continue;
        }
        char completePath[maxCommandLength * 2];
        snprintf(completePath, sizeof(completePath), "%s/%s", path, fileList[i]->d_name);

        struct stat fileDetails;
        if (lstat(completePath, &fileDetails) != 0)
        {
            printf("Error: LSTAT!\n");
            return;
        }

        if (all) // if l flag is set
        {
            // print the permissions
            // directory or file??
            if (S_ISDIR(fileDetails.st_mode))
                printf("d");
            else if (fileDetails.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");

            // read,write,exe permissions of user
            if (fileDetails.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");

            if (fileDetails.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");

            if (fileDetails.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");

            // read,write,exe permissions of group
            if (fileDetails.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");

            if (fileDetails.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");

            if (fileDetails.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");

            // read,write,exe permissions of others
            if (fileDetails.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");

            if (fileDetails.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");

            if (fileDetails.st_mode & S_IXOTH)
                printf("x\t");
            else
                printf("-\t");

            struct passwd *pass = getpwuid(fileDetails.st_uid);
            struct group *grp = getgrgid(fileDetails.st_gid);
            printf("%d\t", fileDetails.st_nlink);
            printf("%8s\t", pass->pw_name); // username
            printf("%8s\t", grp->gr_name);
            printf("%7lld\t", fileDetails.st_size); // file or directories size

            // chronological details
            char time_inf[maxCommandLength];
            struct tm *info = localtime(&fileDetails.st_mtime);
            strftime(time_inf, sizeof(time_inf), "%b %d %H:%M", info);
            printf(" %s\t", time_inf);
            // printf("%d\t", fileDetails.st_blocks);
            // prints the file names in diffrent colors
            // exe - green
            // directories - blue
            // files - white
            if (S_ISDIR(fileDetails.st_mode))
                printf(BLUE "%s\n" DEFAULT, fileList[i]->d_name);
            else if (fileDetails.st_mode & S_IXUSR || fileDetails.st_mode & S_IXGRP || fileDetails.st_mode & S_IXOTH)
                printf(GREEN "%s\n" DEFAULT, fileList[i]->d_name);
            else
                printf(WHITE "%s\n" DEFAULT, fileList[i]->d_name);
        }

        else
        { // prints the file names in diffrent colors
            // exe - green
            // directories - blue
            // files - white
            if (S_ISDIR(fileDetails.st_mode))
                printf(BLUE "%s\n" DEFAULT, fileList[i]->d_name);
            else if (fileDetails.st_mode & S_IXUSR || fileDetails.st_mode & S_IXGRP || fileDetails.st_mode & S_IXOTH)
                printf(GREEN "%s\n" DEFAULT, fileList[i]->d_name);
            else
                printf(WHITE "%s\n" DEFAULT, fileList[i]->d_name);
        }
        free(fileList[i]);
        fileCount--;
        i++;
    }
    free(fileList);
}

void peek(int maxCommandLength, char *command[maxCommandLength], char homeDirectory[maxCommandLength], int arguments)
{
    bool hidden = 0;
    bool all = 0;
    const char *path = ".";

    if (arguments >= 2)
    {
        for (int i = 1; i < arguments; i++)
        {
            if (strcmp(command[i], "-a") == 0)
            {
                hidden = true;
            }
            else if (strcmp(command[i], "-l") == 0)
            {
                all = true;
            }
            else if (strcmp(command[i], "-la") == 0)
            {
                all = true;
                hidden = true;
            }
            else if (strcmp(command[i], "-al") == 0)
            {
                all = true;
                hidden = true;
            }
            else if (command[i][0] == '~')
            {
                char temp[maxCommandLength];
                strcpy(temp, command[i]);
                removeSubstring(temp, "~");
                char org_path[maxCommandLength];
                strcpy(org_path, homeDirectory);
                strcat(org_path, temp);
                path = org_path;
                break;
                // printf("%s\n", path);
            }
            else if (command[i][0] != '-')
            {
                path = command[i];
                break;
            }
            else
            {
                printf("Error - Incorrect Syntax of peek\n");
                return;
            }
        }
    }
    else if (arguments > 4)
    {
        printf("Error - Incorrect Syntax of peek\n");
        return;
    }
    // printf("%s\n", path);
    list(maxCommandLength, path, hidden, all);
}

#include "headers/headers.h"

void saveHistoryToFile(const char history[][MAX_COMMAND_LENGTH], int count, char *location)
{
    FILE *file = fopen(location, "w");
    if (file)
    {
        for (int i = 0; i < count; i++)
        {
            fprintf(file, "%s\n", history[i]);
        }
        fclose(file);
    }
}

void loadHistoryFromFile(char history[][MAX_COMMAND_LENGTH], int *count, char *location)
{
    FILE *file = fopen(location, "r");
    if (file)
    {
        char line[MAX_COMMAND_LENGTH];
        *count = 0;
        while (fgets(line, sizeof(line), file) && *count < MAX_HISTORY_SIZE)
        {
            line[strcspn(line, "\n")] = '\0';
            strcpy(history[*count], line);
            (*count)++;
        }
        fclose(file);
    }
}

bool isCommandInHistory(const char history[][MAX_COMMAND_LENGTH], int count, const char *command)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(history[i], command) == 0)
        {
            return true;
        }
    }
    return false;
}

void storeCommands(char *miniCommand[MAX_COMMAND_LENGTH], char *input, char history[MAX_HISTORY_SIZE][MAX_COMMAND_LENGTH], int *historyCount, char *location)
{
    if (strcmp(miniCommand[0], "pastevents") != 0 && strcmp(input, history[*historyCount - 1]) != 0 && !isCommandInHistory(history, *historyCount, input))
    {
        if (*historyCount == MAX_HISTORY_SIZE)
        {
            for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++)
            {
                strcpy(history[i], history[i + 1]);
            }
            (*historyCount)--;
        }

        char *search = "pastevents execute ";
        char *substring_start = strstr(input, search);
        int exc_ind;
        if (substring_start != NULL)
        {
            int extractedNumber;
            char *number_start = substring_start + strlen(search);

            if (sscanf(number_start, "%d", &extractedNumber) == 1)
            {
                exc_ind = *historyCount - extractedNumber;
                char temp[MAX_COMMAND_LENGTH];
                sprintf(temp, "pastevents execute %d", extractedNumber);
                findAndReplace(input, temp, history[exc_ind]);
            }
        }
        ///////

        substring_start = strstr(input, "; pastevents purge");
        if (substring_start != NULL)
        {
            removeSubstring(input, "; pastevents purge");
        }

        substring_start = strstr(input, ";pastevents purge");
        if (substring_start != NULL)
        {
            removeSubstring(input, ";pastevents purge");
        }

        substring_start = strstr(input, "pastevents purge");
        if (substring_start != NULL)
        {
            removeSubstring(input, "pastevents purge");
        }
        ////////
        if (input[0] == 'p' && input[1] == 'a' && input[2] == 's' && input[3] == 't' && input[4] == 'e' && input[5] == 'v' && input[6] == 'e' && input[7] == 'n' && input[8] == 't')
        {
            substring_start = strstr(input, "pastevents purge;");
            if (substring_start != NULL)
            {
                removeSubstring(input, "pastevents purge;");
            }

            substring_start = strstr(input, "pastevents purge ;");
            if (substring_start != NULL)
            {
                removeSubstring(input, "pastevents purge ;");
            }

            substring_start = strstr(input, "pastevents;");
            if (substring_start != NULL)
            {
                removeSubstring(input, "pastevents;");
            }

            substring_start = strstr(input, "pastevents ;");
            if (substring_start != NULL)
            {
                removeSubstring(input, "pastevents ;");
            }
        }

        ///////////
        substring_start = strstr(input, ";pastevents");
        if (substring_start != NULL)
        {
            removeSubstring(input, ";pastevents");
        }

        substring_start = strstr(input, "; pastevents");
        if (substring_start != NULL)
        {
            removeSubstring(input, "; pastevents");
        }

        substring_start = strstr(input, "pastevents");
        if (substring_start != NULL)
        {
            removeSubstring(input, "pastevents");
        }
        ///////////

        if (strcmp(input, history[*historyCount - 1]) != 0 && !isCommandInHistory(history, *historyCount, input))
        {
            strcpy(history[*historyCount], input);
            (*historyCount)++;
            saveHistoryToFile(history, *historyCount, location);
        }
    }
}
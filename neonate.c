#include "headers.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

int keyboardCapture()
{
    struct termios orig_termios, raw;
    int ch;
    int oldf;

    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    if (tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios))
        die("setattr");
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

pid_t getprocessID()
{
    FILE *file = fopen("/proc/loadavg", "r");
    if (file == NULL)
    {
        perror("Unable to open the file");
        return 1;
    }
    char a[10], b[10], c[10], d[10], e[10];
    fscanf(file, "%s %s %s %s %s", a, b, c, d, e);
    // Close the file
    int res = atoi(e);
    fclose(file);
    return res;
}

void neonate(int maxCommandLength, int arguments, char *miniCommand[maxCommandLength])
{

    if (arguments != 3)
    {
        printf("incorrect syntax!\n");
        return;
    }

    if (strcmp(miniCommand[1], "-n") != 0)
    {
        printf("Incorrect Flags\n");
        return;
    }

    int n = atoi(miniCommand[2]);
    if (n <= 0)
    {
        printf("Invalid time\n");
        return;
    }

    struct termios orig_termios, raw;

    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    while (1)
    {
        pid_t recentID = getprocessID();
        if (recentID != -1)
        {
            printf("%d\n", recentID);
        }
        while (n--)
        {
            if (keyboardCapture())
            {
                char c = getchar();
                if (c == 'x')
                {
                    if (tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios))
                        die("setattr");
                    return;
                }
            }

            sleep(1);
        }
        n = atoi(miniCommand[2]);
    }

    if (tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios))
        die("setattr");

    return;
}
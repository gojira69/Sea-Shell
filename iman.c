#include "headers.h"

#define WEBSITE "man.he.net"
#define PORT "80"

void iMan(char *str)
{
    char man_page[1024];
    sprintf(man_page, "/?topic=%s&section=all", str);

    int socket_file_descriptor;
    struct addrinfo parameters, *server_info, *p_list;
    memset(&parameters, 0, sizeof(parameters));
    parameters.ai_family = AF_UNSPEC;     // IPV4
    parameters.ai_socktype = SOCK_STREAM; // TCP protocol

    // get info about the host
    int gai = getaddrinfo(WEBSITE, PORT, &parameters, &server_info);
    if (gai != 0)
    {
        printf("Error: No such command exists\n");
        return;
    }

    // loop through all the addresses and try to establish connection with any one of them
    for (p_list = server_info; p_list != NULL; p_list = p_list->ai_next)
    {
        socket_file_descriptor = socket(p_list->ai_family, p_list->ai_socktype, p_list->ai_protocol);
        if (socket_file_descriptor == -1)
        {
            printf("Error: SOCKET!\n");
            continue;
        }

        int con_flag = connect(socket_file_descriptor, p_list->ai_addr, p_list->ai_addrlen);
        if (con_flag == -1)
        {
            printf("Error: CONNECT!\n");
            continue;
        }
        break;
    }

    // if the list itself is null, then connection failed
    if (p_list == NULL)
    {
        printf("Connection Failed\n");
        return;
    }

    // prepare a get request
    char get_request[4096];
    sprintf(get_request, "GET %s HTTP/1.1\r\n"
                         "HOST: %s\r\n"
                         "Connection: close\r\n\r\n",
            man_page, WEBSITE);

    // send the get request
    int send_flag = send(socket_file_descriptor, get_request, strlen(get_request), 0);
    if (send_flag == -1)
    {
        printf("Error: SEND!\n");
        return;
    }

    char receivedText[4096], printText[1000000];
    ssize_t bytesReceived;
    int inside_pre_tag = 0;
    int idx = 0;
    while ((bytesReceived = recv(socket_file_descriptor, receivedText, sizeof(receivedText) - 1, 0)) > 0)
    {
        receivedText[bytesReceived] = '\0';

        for (int i = 0; i < bytesReceived; i++)
        {
            if (receivedText[i] == '<')
            {
                if (strncmp(&receivedText[i], "<PRE>", 5) == 0)
                {
                    inside_pre_tag = 1;
                    i += 4;
                    continue;
                }

                if (strncmp(&receivedText[i], "</PRE>", 6) == 0)
                {
                    inside_pre_tag = 0;
                    i += 5;
                    continue;
                }
            }

            if (inside_pre_tag)
            {
                printText[idx++] = receivedText[i];
            }
        }
    }
    if (bytesReceived == -1)
    {
        printf("Error: Invalid command!\n");
        return;
    }
    printText[idx] = '\0';

    if (idx == 0)
    {
        printf("Error: Invalid command!\n");
        return;
    }

    // close the connection
    close(socket_file_descriptor);
    freeaddrinfo(server_info);

    removeHTMLTags(printText);
    printf("%s\n", printText);

    return;
}
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 64
#define PORT 7777

void chat(int sockfd)
{
    char buff[BUFFER_SIZE];
    while (1)
    {
        bzero(buff, sizeof(buff));
        printf("Message: ");
        int n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("Response: %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Disconnected\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Client socket creation failed\n");
        return 0;
    }
    else
    {
        printf("Client socket created\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        printf("Couldn't connect to the server\n");
        return 0;
    }
    else
    {
        printf("Connected to the server\n");
    }

    chat(sockfd);
    close(sockfd);
}

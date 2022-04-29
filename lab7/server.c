#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 64
#define PORT 7777

void chat(int sockfd)
{
    struct sockaddr_in commSocket;
    int len = sizeof(commSocket);
    int connfd = accept(sockfd, (struct sockaddr *)&commSocket, &len);
    if (connfd < 0)
    {
        printf("Server acccept failed\n");
        return;
    }
    else
    {
        printf("Server acccepted the client\n");
    }   
    char buff[BUFFER_SIZE];
    while (1)
	{
		bzero(buff, BUFFER_SIZE);
		read(sockfd, buff, sizeof(buff));
		printf("Message: %s", buff);
		write(sockfd, buff, sizeof(buff));

		if (strncmp("exit", buff, 4) == 0)
		{
			printf("Disconnected\n");
			break;
		}
	} 
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Server socket creation failed\n");
        close(sockfd);
        return 0;
    }
    else
    {
        printf("Server socket created\n");
    }

    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))))
    {
        printf("Socket bind failed\n");
        close(sockfd);
        return 0;
    }
    else
    {
        printf("Socket successfully binded\n");
    }

    if ((listen(sockfd, 4)))
    {
        printf("Listen failed\n");
        close(sockfd);
        return 0;
    }
    else
    {
        printf("Server listening\n");
    }

    chat(sockfd);
    close(sockfd);
}

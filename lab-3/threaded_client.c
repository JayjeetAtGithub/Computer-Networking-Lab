#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 

#define MAX 1024

void driver_func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Enter the expression : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("Result : %s\n\n\n", buff); 
	} 
} 

int main(int argc, char **argv) 
{ 
	int sockfd, connfd, portno; 
	struct sockaddr_in servaddr, cli; 

    if (argc < 2) {
        perror("usage: ./client <port>\n");
        exit(EXIT_FAILURE);
    }

    portno = atoi(argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		perror("socket creation failed...\n"); 
		exit(EXIT_FAILURE); 
	} else {
		printf("Socket successfully created..\n");
        fflush(stdout);
    }
    
	bzero(&servaddr, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(portno); 

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) { 
		perror("connection with the server failed...\n"); 
		exit(EXIT_FAILURE); 
	} else {
		printf("connected to the server..\n"); 
        fflush(stdout);
    }

	driver_func(sockfd); 
	close(sockfd); 
}

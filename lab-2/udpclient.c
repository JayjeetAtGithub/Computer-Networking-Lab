#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAX_SIZE 1024 

int main(int argc, char **argv) { 
	int sockfd, portno, e;
    socklen_t len; 
	char buffer[MAX_SIZE]; 
	struct sockaddr_in   servaddr; 

    if (argc < 2) {
        perror("usage: ./client <port>\n\n");
        exit(EXIT_FAILURE);
    }

    // get port no. from cmd line arguments
    portno = atoi(argv[1]);

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed.\n\n"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// filling server information
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(portno); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 

    // take the remote path input
    printf("enter the remote path: ");
    fflush(stdout);

    fgets(buffer, MAX_SIZE, stdin);

    len = sizeof(servaddr);
	e = sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr, len); 
    if (e < 0) {
        perror("can't send data to server.\n\n");
        exit(EXIT_FAILURE);
    }
    
	bzero(buffer, MAX_SIZE);

	e = recvfrom(sockfd, (char *)buffer, MAX_SIZE, 0, (struct sockaddr *) &servaddr, &len);
	if (e < 0) {
        perror("can't recieve data from server.\n\n");
        exit(EXIT_FAILURE);
    } 

	printf("%s\n", buffer);
	fflush(stdout);

	close(sockfd); 
	return 0; 
} 

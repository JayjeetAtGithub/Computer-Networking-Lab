/*
Name: Jayjeet Chakraborty
Roll No.: 17CS8036
*/

#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 5000 
#define MAXLINE 1024 
int main() 
{ 
	int sockfd; 
	char buffer[MAXLINE]; 

	struct sockaddr_in servaddr; 

	int n, len; 
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("socket creation failed"); 
		exit(0); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 

	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

	printf("enter the string: ");
	fflush(stdout);
	fgets(buffer, MAXLINE, stdin);

	if (connect(sockfd, (struct sockaddr*)&servaddr, 
							sizeof(servaddr)) < 0) { 
		printf("\n Error : Connect Failed \n"); 
	}

	write(sockfd, buffer, sizeof(buffer)); 
	printf("Message from server: ");
        memset(buffer, 0, sizeof(buffer)); 
	read(sockfd, buffer, sizeof(buffer)); 
	puts(buffer); 
	close(sockfd); 
} 


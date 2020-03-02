/*
Name: Jayjeet Chakraborty
Roll No.: 17CS8036
*/

#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8003
#define SA struct sockaddr 

int eval_expr(char expr[MAX]) {
	int a, b, res;
	char op;

	a = expr[0] - '0';
    b = expr[2] - '0';
	op = expr[1];

	if (op == '+') {
		return a + b;
	}

	if (op == '-') {
		return a - b;
	}

	if (op == '*') {
		return a * b;
	}

	if (op == '/') {
		return a / b;
	}

	return -1;
}

// driver function
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int res; 

	for (;;) { 
		bzero(buff, MAX); 

		read(sockfd, buff, sizeof(buff)); 

		res = eval_expr(buff);

		char result_char = (char)res + '0';
	    bzero(buff, MAX); 
        buff[0] = result_char;

		write(sockfd, buff, sizeof(buff)); 

	} 
}

int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	func(connfd); 

	close(sockfd); 
}

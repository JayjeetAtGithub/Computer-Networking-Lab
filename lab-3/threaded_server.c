#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

#define MAX 1024 
#define MAX_CONN 10

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

void *driver_thread(void *arg) { 
	char buff[MAX]; 
	int result; 
	int connfd = (int *)arg;
	for (;;) { 
		bzero(buff, MAX); 

		read(connfd, buff, sizeof(buff)); 

		result = eval_expr(buff);

		char result_char = (char)result + '0';
	    bzero(buff, MAX); 
        buff[0] = result_char;

		write(connfd, buff, sizeof(buff)); 
	}
	close(connfd);
	pthread_exit(NULL);
}

int main(int argc, char **argv) { 
	int sockfd, portno, optval; 
	int connfds[MAX_CONN];
	socklen_t clilen;
	pthread_t tid[MAX_CONN];

	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddrs[MAX_CONN];

	if (argc < 2) {
		perror("usage: ./server <port>\n");
		exit(EXIT_FAILURE);
	} 

	// get the port from the user
	portno = atoi(argv[1]);
	
	// create the socket file descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		perror("socket creation failed...\n"); 
		exit(EXIT_FAILURE); 
	} else {
		printf("socket successfully created..\n");
		fflush(stdout);
	}

	// enable rerunning the server immediately after killing it.
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));
	
	// fill out the server address information
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(portno);
    
	// bind the socket to the server address
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		perror("socket bind failed...\n"); 
		exit(EXIT_FAILURE); 
	} else {
		printf("socket successfully binded..\n");
		fflush(stdout);
	}
    
	// start listening on the socket for connection
	if ((listen(sockfd, 5)) != 0) { 
		perror("listen failed...\n"); 
		exit(EXIT_FAILURE); 
	} else {
		printf("server listening..\n"); 
		fflush(stdout);
	}

	for (int i = 0; i < MAX_CONN; i++) {
		bzero(&cliaddrs[i], NULL);
	}

	for (int i = 0; i < MAX_CONN; i++) {
		clilen = sizeof(cliaddrs[i]);

		// failing in this accept statement.
		connfds[i] = accept(sockfd, (struct sockaddr *)&cliaddrs[i], &clilen); 
		if (connfds[i] < 0) { 
			perror("server acccept failed...\n"); 
			exit(EXIT_FAILURE); 
		} else {
			printf("server acccept the client...\n");
			fflush(stdout);
		}

		pthread_create(&tid[i], NULL, driver_thread, (void *)connfds[i]);
		// pthread_join(&tid[i], NULL);
	}

	// close the server socket file descriptor
	close(sockfd); 
}

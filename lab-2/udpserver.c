/*
Name: Jayjeet Chakraborty
Roll No.: 17CS8036
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <dirent.h>
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAX_SIZE  1024 

char *list_dir(char path[MAX_SIZE]) {
    int len;
    len = strlen(path) - 1;
    char *str;
    str = (char *)malloc(sizeof(char)*len);
    for (int i = 0; i < len; i++){
        *(str + i) = path[i];
    }

    char *result = (char *)malloc(sizeof(char)*MAX_SIZE);

    DIR *d;
    struct dirent *dir;
    d = opendir(str);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            strcat(result, dir->d_name);
        }
        closedir(d);
    } else {
        return "ENOENT: No such file or directory.";
        closedir(d);
    } 
    return result;
}

int main(int argc, char **argv) { 
	int sockfd, portno, e, optval;
    socklen_t clilen;
	char buffer[MAX_SIZE]; 
	struct sockaddr_in servaddr, cliaddr;
    
    if (argc < 2) {
        perror("usage: ./server <port>\n\n");
        exit(EXIT_FAILURE);
    }

    // get port no. from cmd line arguments
    portno = atoi(argv[1]);

	// creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed.\n\n"); 
		exit(EXIT_FAILURE); 
	} 

    // enable rerunning the server immediately after killing it.
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));
	
    // empty the servaddr and cliaddr structs
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// filling server information 
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // use the default IP address
	servaddr.sin_port = htons(portno); // change port to network byte ordering
	
	// bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
		perror("bind failed.\n\n"); 
		exit(EXIT_FAILURE); 
	} else {
        printf("listening on %d \n\n", portno);
        fflush(stdout);
    }
	
    // recv path from client
    clilen = sizeof(cliaddr);

    while (1) {
        // recieve data from client
        bzero(buffer, MAX_SIZE);
        e = recvfrom(sockfd, (char *)buffer, MAX_SIZE, 0, (struct sockaddr *) &cliaddr, &clilen);
        if (e < 0) {
            perror("can't receive data from client.\n\n");
        }

        // list the files in the dir.
        char *result = list_dir(buffer);
        
        // send list of file to client.
        e = sendto(sockfd, result, strlen(result), 0, (const struct sockaddr *) &cliaddr, clilen);
        if (e < 0) {
            perror("can't send data to client.\n\n");
        }
    }
	return 0; 
}

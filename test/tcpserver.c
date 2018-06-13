#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <ctype.h>  
  
#define MAXLINE 512  
  
int main(int argc, char *argv[])  
{  
	struct sockaddr_in servaddr, cliaddr;  
	socklen_t cliaddr_len;  
	int listenfd, connfd;  
	char buf[MAXLINE];  
	char str[INET_ADDRSTRLEN],*port;  
	int i, n;  
	
	if(argc != 2) {  
		printf("usage: %s port \n",argv[0]);  
		exit(1);  
	}  

	port=	argv[1];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);  
	  
	bzero(&servaddr, sizeof(servaddr));  
	servaddr.sin_family = AF_INET;  
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
	servaddr.sin_port = htons(atoi(port));
	
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 20);  
	
	printf("Accepting connections ...\n");
 
	while(1) {  
		cliaddr_len = sizeof(cliaddr);  
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);  

		printf("please input msg:\n");

		fgets(buf,MAXLINE,stdin);

		write(connfd, buf, strlen(buf));  
		close(connfd);
    	}  
}  

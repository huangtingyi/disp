#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
  
#define MAXLINE 512  
  
int main(int argc, char *argv[])  
{  
	struct sockaddr_in servaddr;  
	char buf[MAXLINE];  
	
	int sockfd, n;  
	char *port,*ip;  
	
	if(argc != 3) {  
		fputs("usage: ./client ip,port \n", stderr);  
		exit(1);  
	}  

	ip=	argv[1];
	port=	argv[2];  
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  
	bzero(&servaddr, sizeof(servaddr));  
	servaddr.sin_family = AF_INET;  
	inet_pton(AF_INET, ip, &servaddr.sin_addr);  
	servaddr.sin_port = htons(atoi(port));  
	  
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));  
	 
	while(1){  
		n = read(sockfd, buf, MAXLINE);
		if(n<=0){
			usleep(100);
			continue;
		}  
		printf("Response from server:\n");  
		write(STDOUT_FILENO, buf, n);
	}
	
	close(sockfd);  
	return 0;  
}  

/*
*broadcast_server - 多播服务程序
 */
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MCAST_PORT 8888
#define MCAST_ADDR "234.2.2.2"
#define MCAST_DATA "BROADCAST TEST DATA"
#define MCAST_INTERVAL 3

int main(int argc,char *argv[])
{
	int s,n,i=0,l;
	char sTemp[128];
	struct sockaddr_in mcast_addr;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s == -1){
		perror("socket()");
		return -1;
	}
	
	// 注意， 这个n值很重要
	n = 50;
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&n, sizeof(n));


	memset(&mcast_addr, 0, sizeof(mcast_addr));
	mcast_addr.sin_family = AF_INET;
	mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	mcast_addr.sin_port = htons(MCAST_PORT);

	while(1){
		
		l=sprintf(sTemp,"%s i=%d",MCAST_DATA,i++);
		
		n = sendto(s,sTemp,l,0,(struct sockaddr*)&mcast_addr,sizeof(mcast_addr));

		if(n < 0){
			perror("sendto()");
			return -2;
		}

		printf("send %s to %s port %d n=%d,len=%ld.\n",
			sTemp,MCAST_ADDR,MCAST_PORT,n,strlen(sTemp));

		sleep(MCAST_INTERVAL);
	}

	return 0;
}

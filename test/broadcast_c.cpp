/*
*broadcast_client - 多播客户程序
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
#define MCAST_INTERVAL 3
#define BUFF_SIZE 32

int main(int argc,char *argv[])
{
	int s;
	struct sockaddr_in local_addr;
	int err = -1;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s == -1){
		perror("Socket()");
		return -1;
	}

	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	local_addr.sin_port = htons(MCAST_PORT);

	err = bind(s, (struct sockaddr*)&local_addr,sizeof(local_addr));
	if(err<0){
		perror("bind()");
		return -2;
	}


	int loop = 1;
	err = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	if(err < 0){
		perror("setsockopt():IP_MULTICAST_LOOP");
		return -3;
	}

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = INADDR_ANY;

	err = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	if(err < 0){
		perror("setsockopt():IP_ADD_MEMBERSHIP");
		return -4;
	}

	char buff[BUFF_SIZE];
	int times = 0,addr_len = 0,n = 0;

	for(times = 0; times <50; times++){
		addr_len = sizeof(local_addr);
		memset(buff, 0, BUFF_SIZE);

		printf("recv %s port %d.\n", MCAST_ADDR, MCAST_PORT);
		
		n = recvfrom(s, buff, BUFF_SIZE, 0, (struct sockaddr*)&local_addr, (socklen_t*)&addr_len);
		if(n == -1){
			perror("recvform()");
		}

		printf("Recv %dst message from server:%s\n", times, buff);
		sleep(MCAST_INTERVAL);
	}

	err = setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));
	close(s);
	return 0;
}

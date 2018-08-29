/* test_fdpass.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#include <sys/socket.h> /* for socketpair */


#define MY_LOGO	 "-- Tony Bai"


static int send_fd(int fd, int fd_to_send)
{

	struct iovec    iov[1];
	struct msghdr   msg;
	char	    buf[1];

	union{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	}control_un;

	if (fd_to_send >= 0) {
		msg.msg_control       = (char*)control_un.control;
		msg.msg_controllen    = sizeof(control_un.control);
	}
	else {
		msg.msg_control       = (int*)NULL;
		msg.msg_controllen    = 0;
	}


	msg.msg_name    = NULL;
	msg.msg_namelen = 0;


	iov[0].iov_base = buf;
	iov[0].iov_len  = 1;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;


	struct cmsghdr *cmptr = CMSG_FIRSTHDR(&msg);
	cmptr->cmsg_len = CMSG_LEN(sizeof(int));
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;

	*((int*)CMSG_DATA(cmptr)) = fd_to_send;

	if(sendmsg(fd, &msg, 0) < 0) {
		printf("sendmsg error, errno is %d\n", errno);
		return errno;
	}

printf("send:msg.msg_controllen=%ld,fd=%d.\n",msg.msg_controllen,fd_to_send);

	return 0;
}


static int recv_fd(int fd, int *fd_to_recv)
{
	struct iovec    iov[1];
	struct msghdr   msg;
	char	    buf[1];
	
	union{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	}control_un;


	msg.msg_control       = (char*)control_un.control;
	msg.msg_controllen    = sizeof(control_un.control);

	msg.msg_name    = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = buf;
	iov[0].iov_len  = 1;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;
	
//	((int*)CMSG_DATA(cmptr)) = fd_to_recv;

	if (recvmsg(fd, &msg, 0) < 0) 	return errno;

	struct cmsghdr *cmptr = CMSG_FIRSTHDR(&msg);	
	*fd_to_recv=*((int*)CMSG_DATA(cmptr));

printf("recv:msg.msg_controllen=%ld,fd=%d.\n",msg.msg_controllen,*fd_to_recv);

	if(msg.msg_controllen != sizeof(control_un.control))	*fd_to_recv = -1;


	return 0;
}


int x_sock_set_block(int sock, int on)
{
	int	     val;
	int	     rv;


	val = fcntl(sock, F_GETFL, 0);
	if (on)
		rv = fcntl(sock, F_SETFL, ~O_NONBLOCK&val);
	else
		rv = fcntl(sock, F_SETFL, O_NONBLOCK|val);


	if (rv) 	return errno;

	return 0;
}


int main()
{
	pid_t   pid;
	int     sockpair[2],rv,fd;

	char    fname[256];

	rv = socketpair(AF_UNIX, SOCK_STREAM, 0, sockpair);
	if (rv < 0) {
		printf("Call socketpair error, errno is %d\n", errno);
		return errno;
	}


	pid = fork();
	if (pid == 0) {
		/* in child */
		close(sockpair[1]);


		for ( ; ; ) {
			rv = x_sock_set_block(sockpair[0], 1);
			if (rv != 0) {
				printf("[CHILD]: x_sock_set_block error, errno is %d\n", rv);
				break;
			}

			fd=0;
			rv = recv_fd(sockpair[0], &fd);
			if (rv < 0) {
				printf("[CHILD]: recv_fd error, errno is %d\n", rv);
				break;
			}


			if (fd < 0) {
				printf("[CHILD]: child process exit normally!\n");
				break;
			}


		       /* 处理fd描述符对应的文件 */
			rv = write(fd, MY_LOGO, strlen(MY_LOGO));
			if (rv < 0) {
				printf("[CHILD]: write error, errno is %d\n", rv);
			} else {
				printf("[CHILD]: append logo successfully\n");
			}
			close(fd);
		}


		exit(0);
	}


	/* in parent */
	for ( ; ; ) {
		memset(fname, 0, sizeof(fname));
		printf("[PARENT]: please enter filename:\n");
		scanf("%s", fname);


		if (strcmp(fname, "exit") == 0) {
			rv = send_fd(sockpair[1], -1);
			if (rv < 0) {
				printf("[PARENT]: send_fd error, errno is %d\n", rv);
			}
			break;
		}


		fd = open(fname, O_RDWR | O_APPEND);
		if (fd < 0) {
			if (errno == ENOENT) {
				printf("[PARENT]: can't find file '%s'\n", fname);
				continue;
			}
			printf("[PARENT]: open file error, errno is %d\n", errno);
		}


		rv = send_fd(sockpair[1], fd);
		if (rv != 0) {
			printf("[PARENT]: send_fd error, errno is %d\n", rv);
		}


		close(fd);
	}


	wait(NULL);
	return 0;
}
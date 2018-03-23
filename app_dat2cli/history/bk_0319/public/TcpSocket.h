#ifndef TcpSocket_H_HEADER_INCLUDED_BE6E2CF5
#define TcpSocket_H_HEADER_INCLUDED_BE6E2CF5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <string>
using namespace std;

//##ModelId=4314101B03E3
//##Documentation
//## TCP/IP TcpSocket封装
class TcpSocket
{
public:
	//##ModelId=4314101E02A7
	//##Documentation
	//## 服务器侦听(服务端使用)
	int listen(
	//##Documentation
	//## 端口号
			int iPortID,
			//##Documentation
			//## 服务进程连接后，是否fork自身.
			bool bForkFlag = false);

	//## 服务器侦听(服务端使用)
	//增加 struct sockaddr_in NewAddr 返回客户端IP
	int listen(
	//##Documentation
	//## 端口号
			int iPortID,
			//##Documentation
			//## 服务进程连接后，是否fork自身.
			struct sockaddr_in& NewAddr, bool bForkFlag = false);
	//##ModelId=4314101E02AA
	//##Documentation
	//## 连接TcpSocket(客户端使用)
	int connect(
	//##Documentation
	//## 远程服务器IP
			char *sRemoteHostIP,
			//##Documentation
			//## 远程服务器端口
			int iRemotePortID);

	int connect(
	//##Documentation
	//## 远程服务器IP
			char *sRemoteHostIP,
			//##Documentation
			//## 远程服务器端口
			int iRemotePortID,
			//connect超时时间，单位秒
			int time_out_time);

	int setBlock();

	//##ModelId=4314101E02AD
	TcpSocket();

	//##ModelId=4314101E02AE
	virtual ~TcpSocket();

	int read(unsigned char *sBuffer, int iLen, unsigned long timeout_val);
	//##ModelId=432E7B0E019F
	int read(
	//##Documentation
	//## 读取TcpSocket存放的缓冲
			unsigned char *sBuffer,
			//##Documentation
			//## 指定读取长度
			int iLen);

	//##ModelId=432E7B5303C5
	int write(
	//##Documentation
	//## 写入TcpSocket的内容地址
			unsigned char *sBuffer,
			//##Documentation
			//## 写入TcpSocket长度
			int iLen);

	int Sendn(char *buf, int len, int timeout_val = 10);

	int CloseListenSocket();
	void Close();

	//## TcpSocketID
	int m_iListenSocket;
	int m_iTcpSocketID;

	int send(unsigned char *sBuffer, int iLen);

	char * getClientAddr(char *sClientAddr, int iLen);

	//bType : true 为发送缓冲区  false 为接收缓冲区
	//iLen:	缓冲区大小
	int setSocketBufferLen(bool bType, int &iLen);
	int getSocketBufferLen(bool bType, int &iLen);

private:
	//##ModelId=4314101E029F
	//##Documentation
	//## TcpSocketID
//    int m_iTcpSocketID;
};

#endif /* TcpSocket_H_HEADER_INCLUDED_BE6E2CF5 */

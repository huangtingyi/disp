#ifndef __DAT2CLI_OLD_H__
#define __DAT2CLI_OLD_H__

#include <vector>
#include <map>
#include "TcpSocket.h"

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <thread>
#include <time.h>
#include<sys/time.h>

#include "CSemaphore.h"
#include "MessageQueue.h"
#include "mktdata.pb.h"

#include<signal.h>
#include "TimerController.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost;

#define ERROR_TRANSPORT -1;
#define SUCC 0
#define SOCKET_DATALEN 8192

#include "dat2cli_supp.h"

class Dat2Client
{
public:
	enum
	{
		MAX_MSG_LENGTH = 2048, MAX_MSG_BLOCK = 10
	};

public:

	Dat2Client();
	~Dat2Client();
	int run(int argc, char *argv[]); //继承进程框架的run函数

private:

	char m_sRecvBuffer[SOCKET_DATALEN]; //接收缓冲区
	char m_sSendBuffer[SOCKET_DATALEN]; //发送缓冲区

	int m_iPid; //收发进程记录与它配对应的PID 主进程记录为0

	int m_iSetSocketFlag;
	char m_sUserPrivJsonPath[MY_MAX_PATH];
	char m_sOutDispJsonPath[MY_MAX_PATH];
	char m_sOutDispLogPath[MY_MAX_PATH];
	char m_sCfgJsonPath[MY_MAX_PATH];
	char m_sWorkRootPath[MY_MAX_PATH];
	char m_sWriteUser[MY_MAX_PATH];
	
	
	ServerInfo_t m_Info;

	int Init();

	int m_iCheckIp; //是否校验IP 1-校验 0-不校验

private:
	
	MapUserAuth	m_mapUserAuth;	//key为用户名，对象为 UserAuth_t

	ConnectClient_t	m_Conn;

	TimerController *m_pTimerController;
	boost::asio::io_service m_io;
	std::thread 	*m_pThread;

	CSemaphore	m_semLock;

private:
	MessageQueue *m_poSysMQ; //总控队列
	MessageQueue *m_poDataMQ; //数据队列

	bool isValidHost(const char *sIp); //判断接入的远程主机是否是受信任的主机IP
	int RecvCliSockRequestAndProcess(char sDestIp[],int iPort);
	int RecvMqAndDisp2Cli(char sDestIp[],int iPort);;
	bool dealCommand(string &msg,char sDestIp[],int iPort);
	bool setSubscrible(const SubscribeRequest &req);
	bool addReduceCodes(const vector<uint32_t> &codes, const bool addReduce);
	bool writeDispJson();

	void logout();
	bool isLogined(string sUserName);
	void handleTimeOut();
public:
	static bool m_bReadSysMq; //是否读取总控队列
	static bool m_bExitFlag ;
};

extern int (*Write2CliFile)(char sWorkRoot[],char sUserName[],string &str);

#endif

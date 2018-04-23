#ifndef _DAT2CLIENT_H_
#define _DAT2CLIENT_H_

//#include <ace/OS.h>
//#include <ace/Get_Opt.h>
#include <vector>
#include <map>
//#include "diameter_parser_api.h"
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
#include "MqMonitorMgr.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost;

#define ERROR_TRANSPORT -1;
#define SUCC 0
#define SOCKET_DATALEN 8192
#define MAX_PATH 512

struct Privalige
{
	string m_sIp;
	string m_sUser;
	int m_iMqID;
	string password;
	set<BizCode> privl;
};
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

	char m_sRecvBuffer[SOCKET_DATALEN]; //发送接收缓冲区
	char m_sSendBuffer[SOCKET_DATALEN]; //发送缓冲区

	int m_iPid; //收发进程记录与它配对应的PID 主进程记录为0

	char m_sIpAddr[30];
	int m_iDebugFlag;
	int m_iSetSocketFlag;

	int m_iSemLockKey;
	char m_sUserPrivJsonPath[MAX_PATH];
	char m_sOutDispJsonPath[MAX_PATH];
	char m_sCfgJsonPath[MAX_PATH];
	char m_sWorkRootPath[MAX_PATH];
	char m_sWriteUser[MAX_PATH];
	int m_iPort; //侦听端口
	int m_iSocketSendLen;
	int m_iSocketRecvLen;
	int m_iSysMqKey;
	int m_iSysMqMaxLen;
	int m_iSysMqMaxNum;


	int Init();
	int diffTimeStr(char *curr_time_str, const char *last_time_str);

	int m_iCheckIp; //是否校验IP 1-校验 0-不校验

	//OperationLoad m_cOperationLoad;
private:
	map<string, Privalige> m_mapPrivl; //key:用户名
	unsigned int m_secHeartbeat;
	string m_sUserName;

	vector<uint32_t> m_allCodes; //这个得其他模块同步过来

	set<BizCode> m_Subscribed;

	//每个客户端连接所对应的超时检测器
	////TimerHeartbeat m_Timer;
	int m_iMqID;
	bool m_subSomeCodes ;
	set<uint32_t> m_codes; //只订阅这些证券代码

	TimerController *m_pTimerController;
	boost::asio::io_service m_io;
	std::thread *m_pThread;
#ifdef DEBUG_ONE
	map<BizCode,long> m_mapBizStat;
	long m_lRecvNum;
	long m_lSendNum;
#endif

	CSemaphore *m_poDataLock;

private:
	bool isValidHost(const char *sIp); //判断接入的远程主机是否是受信任的主机IP
	int RecvCliSockRequestAndProcess();
	int RecvMqAndDisp2Cli();

	MessageQueue *m_poSysMQ; //总控队列
	MessageQueue *m_poDataMQ; //数据队列

	bool dealCommand(string &msg);

	bool setSubscrible(const SubscribeRequest &req);
	bool addReduceCodes(const vector<uint32_t> &codes, const bool addReduce);
	bool writeDispJson();
	
	void logout();
	bool isLogined(string sUserName);
	void handleTimeOut();
	void SetProcessSignal();
	void GetSysDate(char sSysDate[]);
	void startMonitorMq();
	static void runMonitorMqThread(const int iMqid);
public:
	static bool m_bReadSysMq; //是否读取总控队列
	static bool m_bExitFlag ;
};

#endif


#ifndef __DAT2CLI_SUPP_H__
#define __DAT2CLI_SUPP_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <set>
#include <map>
#include <string>
#include <sys/stat.h>
#include <stdarg.h>

using namespace std;

#define MY_MAX_PATH 512

#include "mktdata.pb.h"


typedef struct ServerInfoStruct
{
	uint32_t iSleep;
	uint32_t iSemLockKey;
	uint32_t iPort; //侦听端口
	uint32_t iSocketSendLen;
	uint32_t iSocketRecvLen;
	uint32_t iSysMqKey;
	uint32_t iSysMqMaxLen;
	uint32_t iSysMqMaxNum;
	uint32_t iHeartBeat;
} ServerInfo_t;

//服务端用户权限表
typedef struct UserAuthStruct
{
	string  strIp;			//客户端IP限制
	string  strUserName;		//客户端登录用户名
	string  strPassword;		//客户端登录密码
	int	iMqId;			//客户端对应的MQID
	set <int>	setAuth;	//客户端对应的业务权限集
} UserAuth_t;
//连接客户端信息表
typedef struct ConnectClientStruct {
	char	sUserName[32];		//连接客户端用户名
	char	sDestIp[32];		//字符串形式的连接客户端IP
	int	iPort;			//连接客户端端口号
	int	iPid;			//客户端服务进程的pid
	string  strUserName;		//客户端用户名
	string	strIp;			//连接客户端IP
	int	iMqId;			//客户端对应的后台的mqid
	set<int>	setAllSubs;	//所有可供订阅的业务
	set<int>	setSubscribed;	//当前已订购的业务
	set<int>	setSubsCode;	//当前已订购的股票代码
	map<int,long>	mapSubsStat;	//分业务统计发送记录数
	long		lRecvCnt;	//总接收数
	long		lSendCnt;	//总发送数
} ConnectClient_t;

typedef std::map<string, UserAuth_t> MapUserAuth;
typedef std::map<string, ConnectClient_t> MapConnectClient;


int ReadServerInfo(char sServerInfoPath[], ServerInfo_t &Info);
//将user_privilege.json文件读到map结构中
int ReadUserAuth2Map(char sUserAuthPath[], MapUserAuth &mapUserAuth);

//将bizCode和pbmsg信息打包成为串 格式：2字节长度+1字节bizCode+pb->SerializeToString结果，长度为 1字节+序列化串
void addBizcode(string &output, const google::protobuf::Message &pbmsg, BizCode bizCode);
//将msgInput的第一个字段取出，作为bizCode后面的串作为pbmsg
void getBizcode(BizCode &bizCode, string &pbmsg, const string &msgInput);
void MyBin2HexStr(char *buf,int len,char sTemp[]);
int GetHostTimeX(char sHostTime[15],char sMiniSec[4]);
void PrintHexBuf(char *buf, int size);

bool LogDispJson(char sInfo[],char sMsg[],char sDispPath[],char sDispLog[],char sErrInfo[]);
int UserInWriteUser(char sUserName[],char sWriteUser[]);
int MyWrite2CliFile(char sWorkRoot[],char sUserName[],string &str);

long lFileSize(char sFileName[]);//获取文件名为filename的文件大小。
time_t tFileModifyTime(char sFileName[]);

void printf_dt(const char *fmt,...);

extern int (*Write2CliFile)(char sWorkRoot[],char sUserName[],string &str);


#endif

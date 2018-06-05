#ifndef __CALLSUPP_H__
#define __CALLSUPP_H__
#define MAX_USER_NAME_LEN 64
#define MAX_CLIENT_CNT 1024
#ifndef MAX_STOCK_CODE
#define MAX_STOCK_CODE	1000000
#endif


#include <iostream>
#include <string>

struct UserStruct
{
	struct UserStruct *pNext;
	char sUserName[MAX_USER_NAME_LEN];
	int iParam;	//订购参数
	int iMqId;
	int iMqPos;
	int iStockCode;
	int iSubscribed;
	struct UserStruct *pFreeNext;
};


struct DispRuleStruct
{
	struct UserStruct *PMALL;
	struct UserStruct *PTALL;
	struct UserStruct *PQALL;
	struct UserStruct *POALL;
	struct UserStruct *PDALL;
	struct UserStruct *AMUSER[MAX_STOCK_CODE];
	struct UserStruct *ATUSER[MAX_STOCK_CODE];
	struct UserStruct *AQUSER[MAX_STOCK_CODE];
	struct UserStruct *AOUSER[MAX_STOCK_CODE];
	struct UserStruct *ADUSER[MAX_STOCK_CODE];
};

extern struct LimitUpDownStruct LIMIT[MAX_STOCK_CODE];

void LockWorkThread();
void UnLockWorkThread();
int IsWorkThreadLock();

void InitUserArray(char sDispName[],struct DispRuleStruct *p);
void RefreshUserArray(char sDispName[],struct DispRuleStruct *p);

int WatchFileCloseWriteAndLock(char sFileName[]);

void SendMsg2Cli(int iStockCode,char cType,string& str);

bool ValidShStockCode(char sStockCode[]);
bool ValidSzStockCode(char sStockCode[]);
bool ValidStockCode(char szWinCode[]);

//#include "QTSStruct.h"

extern struct DispRuleStruct R;
extern char sRefreshDispName[1024];

#define MY_AM_MARKET_BEGIN_TIME	90000000
#define MY_AM_MARKET_OPEN_TIME	93000000
#define MY_AM_MARKET_CLOSE_TIME	113000000
#define MY_PM_MARKET_OPEN_TIME	130000000
#define MY_PM_MARKET_CLOSE_TIME	150000000
#define MY_PM_MARKET_STOP_TIME	153000000
#define MY_DAY_END_TIME		240000000

//根据, 指定回放开始时间位置, 算出做完本次回放需要多长时间，单位（毫秒）
int GetReplayCostMSec(int nBgnTime);
//程序启动后，当前还剩余多少毫秒可以使用
int GetReplayDayLeftMSec(int nStartTime);
//程序启动后，预计程序在什么时间结束回放
int GetReplayEndTime(int nStartTime,int iCostMSec);
//根据计算得出的：当前对应的回放位置（距离：MY_PM_MARKET_STOP_TIME的毫秒数），
//换算出ReplayTime的数值为
int GetReplayTimeByLeftMSec(int iEndLeftMSec);
//将程序运行当前时间，映射到回放位置上去
int nGetReplayTimeByCur(int nCurTime,int nStartTime,int nEndTime,
	int iCostMSec,int iDayLeftMSec);

long lFileSize(char sFileName[]);//获取文件名为filename的文件大小。

extern unsigned int    nD31TradeTime;

#endif  //__CALLSUPP_H__

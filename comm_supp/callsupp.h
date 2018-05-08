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


#endif  //__CALLSUPP_H__

#ifndef __CALLSUPP_H__
#define __CALLSUPP_H__
#define MAX_USER_NAME_LEN 64
#define MAX_CLIENT_CNT 1024
#ifndef MAX_STOCK_CODE
#define MAX_STOCK_CODE	1000000
#endif


struct UserStruct
{
	struct UserStruct *pNext;
	char sUserName[MAX_USER_NAME_LEN];
	int iMqId;
	int iMqPos;
};


struct DispRuleStruct
{
	struct UserStruct *PMALL;
	struct UserStruct *PTALL;
	struct UserStruct *PQALL;
	struct UserStruct *POALL;
	struct UserStruct *AMUSER[MAX_STOCK_CODE];
	struct UserStruct *ATUSER[MAX_STOCK_CODE];
	struct UserStruct *AQUSER[MAX_STOCK_CODE];
	struct UserStruct *AOUSER[MAX_STOCK_CODE];
};

extern struct LimitUpDownStruct LIMIT[MAX_STOCK_CODE];

void LockWorkThread();
void UnLockWorkThread();
int IsWorkThreadLock();

void InitUserArray(char sDispName[],struct DispRuleStruct *p);
void RefreshUserArray(char sDispName[],struct DispRuleStruct *p);

int WatchFileCloseWriteAndLock(char sFileName[]);

#include "mktdata.pb.h"  
#include "TDFAPIStruct.h"


void TDF_MARKET_DATA2MktData(MktData &output, const TDF_MARKET_DATA &src);
void TDF_TRANSACTION2Transaction(Transaction &output, const TDF_TRANSACTION &src);
void TDF_ORDER2Order(Order &output, const TDF_ORDER&src);
void TDF_ORDER_QUEUE2Order_queue(Order_queue &output, const TDF_ORDER_QUEUE&src);

void SendMsg2Cli(int iStockCode,char cType,string& str);


extern struct DispRuleStruct R;
extern char sRefreshDispName[1024];


#endif  //__CALLSUPP_H__

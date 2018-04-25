#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "TDFAPI.h"
#include "NonMDMsgDecoder.h"
#include "TDFAPIInner.h"


const char* svr_ip = "114.80.154.34";
int svr_port = 6221;

#define MIN(x, y) ((x)>(y)?(y):(x))

void RecvData(THANDLE hTdf, TDF_MSG* pMsgHead);

void RecvSys(THANDLE hTdf, TDF_MSG* pSysMsg);

void DumpScreenMarket(TDF_MARKET_DATA* pMarket, int nItems);
void DumpScreenFuture(THANDLE hTdf,TDF_FUTURE_DATA* pFuture, int nItems);
void DumpScreenIndex(TDF_INDEX_DATA* pIndex, int nItems);
void DumpScreenTransaction(TDF_TRANSACTION* pTransaction, int nItems);
void DumpScreenOrder(TDF_ORDER* pOrder, int nItems);
void DumpScreenOrderQueue(TDF_ORDER_QUEUE* pOrderQueue, int nItems);

#define ELEM_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))
#define SAFE_STR(str) ((str)?(str):"")
#define SAFE_CHAR(ch) ((ch) ? (ch) : ' ')
	
int MY_INFO_CNT=3000;


char* chararr2str(char* szBuf, int nBufLen, char arr[], int n)
{
	int nOffset = 0;
	for (int i=0; i<n; i++)
	{
		nOffset += snprintf(szBuf+nOffset, nBufLen-nOffset, "%d(%c) ", arr[i], SAFE_CHAR(arr[i]));
	}
	return szBuf;
}

char* intarr2str(char* szBuf, int nBufLen, int arr[], int n)
{
	int nOffset = 0;
	for (int i=0; i<n; i++)
	{
		nOffset += snprintf(szBuf+nOffset, nBufLen-nOffset, "%d ", arr[i]);
	}
	return szBuf;
}

char* int64arr2str(char* szBuf, int nBufLen, int64_t arr[], int n)
{
	int nOffset = 0;
	for (int i=0; i<n; i++)
	{
		nOffset += snprintf(szBuf+nOffset, nBufLen-nOffset, "%ld ", arr[i]);
	}
	return szBuf;
}


int main(int argc, char** argv)
{

	//TDF_SetEnv(TDF_ENVIRON_HEART_BEAT_INTERVAL, 10);
	//TDF_SetEnv(TDF_ENVIRON_MISSED_BEART_COUNT, 2);
	//TDF_SetEnv(TDF_ENVIRON_OPEN_TIME_OUT, 0);

#ifdef MULTI_CONS					//可以同时连接多个服务器，取多个服务器中最新的行情传上来，目前支持2路
	TDF_OPEN_SETTING_EXT settings;

	memset((void*)&settings, 0, sizeof(settings));
	strncpy(settings.siServer[0].szIp, svr_ip, sizeof(settings.siServer[0].szIp)-1);
	sprintf(settings.siServer[0].szPort, "%d",svr_port);
	strncpy(settings.siServer[0].szUser, "dev_test", sizeof(settings.siServer[0].szUser)-1);
	strncpy(settings.siServer[0].szPwd,  "dev_test", sizeof(settings.siServer[0].szPwd)-1);

	settings.nServerNum = 1; //必须设置： 有效的连接配置个数（当前版本应<=2)

	settings.pfnMsgHandler = RecvData; //设置数据消息回调函数
	settings.pfnSysMsgNotify = RecvSys;//设置系统消息回调函数
	settings.szMarkets = "SZ-2-0";	  //需要订阅的市场列表

	settings.szSubScriptions = "000001.SZ"; //"600030.SH"; //600030.SH;104174.SH;103493.SH";	//需要订阅的股票,为空则订阅全市场
	settings.nTime = 0;//请求的时间，格式HHMMSS，为0则请求实时行情，为0xffffffff从头请求
	settings.nTypeFlags = DATA_TYPE_NONE; //请求的品种
	TDF_ERR nErr = TDF_ERR_SUCCESS;
	THANDLE hTDF = NULL;

	hTDF = TDF_OpenExt(&settings, &nErr);
#else
	TDF_OPEN_SETTING settings;

	memset((void*)&settings, 0, sizeof(settings));

	strcpy(settings.szIp,	svr_ip);
	sprintf(settings.szPort, "%d",svr_port);
	strcpy(settings.szUser, "TD1007972005");
	strcpy(settings.szPwd,  "87613480");
	settings.pfnMsgHandler = RecvData; //设置数据消息回调函数
	settings.pfnSysMsgNotify = RecvSys;//设置系统消息回调函数
	settings.szMarkets = "SZ-2-0;SH-2-0";	  //需要订阅的市场列表

	settings.szSubScriptions = ""; //"600030.SH"; //600030.SH;104174.SH;103493.SH";	//需要订阅的股票,为空则订阅全市场
	settings.nTime = 0;//请求的时间，格式HHMMSS，为0则请求实时行情，为0xffffffff从头请求
	settings.nTypeFlags = (DATA_TYPE_NONE|DATA_TYPE_TRANSACTION|DATA_TYPE_ORDER|DATA_TYPE_ORDERQUEUE); //请求的品种
	TDF_ERR nErr = TDF_ERR_SUCCESS;
	THANDLE hTDF = NULL;

	hTDF = TDF_Open(&settings, &nErr);
#endif
	if (hTDF==NULL){
		printf("TDF_Open return error: %d\n", nErr);
	}else{
		printf(" Open Success!\n");
	}
	// Press any key to exit
	getchar();
	TDF_Close(hTDF);
	return 0;
}


#define GETRECORD(pBase, TYPE, nIndex) ((TYPE*)((char*)(pBase) + sizeof(TYPE)*(nIndex)))
#define PRINTNUM  1
static int recordNum = 5;

void RecvData(THANDLE hTdf, TDF_MSG* pMsgHead)
{

	static int iCount=0,iPreCnt=0;
	
	iCount++;

	if((iCount-iPreCnt)>MY_INFO_CNT){
		iPreCnt=iCount;
		printf("--------------------------------------receive count=%d,pid=%d.\n",iCount,getpid());
	}


	if (!pMsgHead->pData) return;

	unsigned int nItemCount = pMsgHead->pAppHead->nItemCount;
//	unsigned int nItemSize = pMsgHead->pAppHead->nItemSize;
	if (!nItemCount) return;

	recordNum++;
	switch(pMsgHead->nDataType)
	{
	case MSG_DATA_MARKET:
	{
		if (recordNum > PRINTNUM){
			recordNum = 0;
			DumpScreenMarket((TDF_MARKET_DATA*)pMsgHead->pData, nItemCount);
		}

	}
	break;
	case MSG_DATA_FUTURE:
	{
		if (recordNum > PRINTNUM){
			recordNum = 0;
			DumpScreenFuture(hTdf,(TDF_FUTURE_DATA*)pMsgHead->pData, nItemCount);
		}
	}
	break;

	case MSG_DATA_INDEX:
	{
		if (recordNum > PRINTNUM){
			recordNum = 0;
			DumpScreenIndex((TDF_INDEX_DATA*)pMsgHead->pData, nItemCount);
		}
	}
	break;
	case MSG_DATA_TRANSACTION:
	{
		if (recordNum > PRINTNUM){
			recordNum = 0;
			DumpScreenTransaction((TDF_TRANSACTION*)pMsgHead->pData, nItemCount);
		}
	}
	break;
	case MSG_DATA_ORDERQUEUE:
	{
		if (recordNum > PRINTNUM){
			recordNum = 0;
			DumpScreenOrderQueue((TDF_ORDER_QUEUE*)pMsgHead->pData, nItemCount);
		}
		TDF_ORDER_QUEUE* pLastOrderQueue = GETRECORD(pMsgHead->pData,TDF_ORDER_QUEUE, nItemCount-1);
/*
		printf( "接收到委托队列记录:代码：%s, 业务发生日:%d, 时间:%d, 委托价格:%d，订单数量:%d \n",
		 	pLastOrderQueue->szWindCode,
		 	(int)pLastOrderQueue->nActionDay,
		 	(int)pLastOrderQueue->nTime,
		 	(int)pLastOrderQueue->nPrice,
		 	(int)pLastOrderQueue->nOrders);
*/
	}
	break;
	case MSG_DATA_ORDER:
	{
		if (recordNum > PRINTNUM){
			recordNum = 0;
			DumpScreenOrder((TDF_ORDER*)pMsgHead->pData, nItemCount);
		}

	}
	break;
	default:
	break;
	}
}

void RecvSys(THANDLE hTdf, TDF_MSG* pSysMsg)
{
	if (!pSysMsg ||! hTdf) return;

	switch (pSysMsg->nDataType)
	{
	case MSG_SYS_DISCONNECT_NETWORK:
	{
			printf("MSG_SYS_DISCONNECT_NETWORK\n");
	}
	break;
	case MSG_SYS_CONNECT_RESULT:
	{
		TDF_CONNECT_RESULT* pConnResult = (TDF_CONNECT_RESULT*)pSysMsg->pData;
		if (pConnResult && pConnResult->nConnResult)
			printf("connect: %s:%s user:%s, password:%s suc!\n", pConnResult->szIp, pConnResult->szPort, pConnResult->szUser, pConnResult->szPwd);
		else
			printf("connect: %s:%s user:%s, password:%s fail!\n", pConnResult->szIp, pConnResult->szPort, pConnResult->szUser, pConnResult->szPwd);
	}
	break;
	case MSG_SYS_LOGIN_RESULT:
	{
		TDF_LOGIN_RESULT* pLoginResult = (TDF_LOGIN_RESULT*)pSysMsg->pData;
		if (pLoginResult && pLoginResult->nLoginResult){
			printf("login suc:info:%s, nMarkets:%d\n", pLoginResult->szInfo, pLoginResult->nMarkets);
			for (int i=0; i<pLoginResult->nMarkets; i++)
				printf("market:%s, dyn_date:%d\n", pLoginResult->szMarket[i], pLoginResult->nDynDate[i]);
		}
		else	printf("login fail:%s\n", pLoginResult->szInfo);
	}
	break;
	case MSG_SYS_CODETABLE_RESULT:
	{
		TDF_CODE_RESULT* pCodeResult = (TDF_CODE_RESULT*)pSysMsg->pData;
		if (pCodeResult ){
			printf("get codetable:info:%s, num of market:%d\n", pCodeResult->szInfo, pCodeResult->nMarkets);
			for (int i=0; i<pCodeResult->nMarkets; i++){
				printf("market:%s, codeCount:%d, codeDate:%d\n", pCodeResult->szMarket[i], pCodeResult->nCodeCount[i], pCodeResult->nCodeDate[i]);

				if (1){
						//CodeTable
					TDF_CODE* pCodeTable;
					unsigned int nItems;
					TDF_GetCodeTable(hTdf, pCodeResult->szMarket[i], &pCodeTable, &nItems);
					printf("nItems =%d\n",nItems);
					
					/**
					for (int i=0; i < (int)nItems; i++){
						TDF_CODE& code = pCodeTable[i];
							printf("windcode:%s, code:%s, market:%s, name:%s, nType:0x%x\n",code.szWindCode, code.szCode, code.szMarket, code.szCNName, code.nType);
					}
					*/
					TDF_FreeArr(pCodeTable);
				}
			}
		}
	}
	break;
	case MSG_SYS_QUOTATIONDATE_CHANGE:
	{
		TDF_QUOTATIONDATE_CHANGE* pChange = (TDF_QUOTATIONDATE_CHANGE*)pSysMsg->pData;
		if (pChange)
			printf("MSG_SYS_QUOTATIONDATE_CHANGE: market:%s, nOldDate:%d, nNewDate:%d\n", pChange->szMarket, pChange->nOldDate, pChange->nNewDate);

	}
	break;
	case MSG_SYS_MARKET_CLOSE:
	{
		TDF_MARKET_CLOSE* pCloseInfo = (TDF_MARKET_CLOSE*)pSysMsg->pData;
		if (pCloseInfo)
			printf("MSG_SYS_MARKET_CLOSE\n");
	}
	break;
	case MSG_SYS_HEART_BEAT:
	{
		printf("MSG_SYS_HEART_BEAT...............\n");
	}
	break;
	default:
		//assert(0);
		break;
	}
}
void DumpScreenMarket(TDF_MARKET_DATA* pMarket, int nItems)
{
	static int iCount=0,iPreCnt=0;
	
	iCount+=nItems;

	if((iCount-iPreCnt)>MY_INFO_CNT){
		iPreCnt=iCount;
	
		printf("-------- Market, Count:%d --------\n", iCount);
	}
}

void DumpScreenFuture(THANDLE hTdf,TDF_FUTURE_DATA* pFuture, int nItems)
{
	static int iCount=0,iPreCnt=0;
	
	iCount+=nItems;

	if((iCount-iPreCnt)>MY_INFO_CNT){
		iPreCnt=iCount;
		printf("-------- Future, Count:%d --------\n", iCount);
	}
}

void DumpScreenIndex(TDF_INDEX_DATA* pIndex, int nItems)
{
	static int iCount=0,iPreCnt=0;
	
	iCount+=nItems;

	if((iCount-iPreCnt)>MY_INFO_CNT){
		iPreCnt=iCount;

		printf("-------- Index, Count:%d --------\n", iCount);
	}
}

void DumpScreenTransaction(TDF_TRANSACTION* pTransaction, int nItems)
{
	static int iCount=0,iPreCnt=0;
	
	iCount+=nItems;

	if((iCount-iPreCnt)>MY_INFO_CNT){
		iPreCnt=iCount;

		printf("-------- Transaction, Count:%d --------\n", iCount);
	}
}

void DumpScreenOrder(TDF_ORDER* pOrder, int nItems)
{
	static int iCount=0,iPreCnt=0;
	
	iCount+=nItems;

	if((iCount-iPreCnt)>MY_INFO_CNT){
		iPreCnt=iCount;

		printf("-------- Order, Count:%d --------\n", iCount);
	}
}

void DumpScreenOrderQueue(TDF_ORDER_QUEUE* pOrderQueue, int nItems)
{
	static int iCount=0,iPreCnt=0;
	
	iCount+=nItems;

	if((iCount-iPreCnt)>MY_INFO_CNT){
		iPreCnt=iCount;

		printf("-------- OrderQueue, Count:%d --------\n", iCount);
	}

}

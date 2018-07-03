#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "TDFAPI.h"
#include "NonMDMsgDecoder.h"
#include "TDFAPIInner.h"
#include "wwtiny.h"

#include <cstring>
#include <cstdint>

#include <stdlib.h>
#include <unistd.h>
#include <boost/property_tree/json_parser.hpp>
#include "IoService.h"
#include "TinyThread.h"

#include "public.h"

#include <pthread.h>


using namespace std;

#include "callsupp.h"

#include "tdf_supp.h"

#include "CallBackBase.h"

#include "wwtiny.h"

#define ELEM_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))
#define SAFE_STR(str) ((str)?(str):"")
#define SAFE_CHAR(ch) ((ch) ? (ch) : ' ')
	
int MY_INFO_CNT=3000;

int iDebugFlag=0,iWriteFlag=0;
char sCfgJsonName[1024],sDispName[1024],sPrivilegeName[1024],sWorkRoot[1024],sWorkD31[1024];

FileNameSet m_m_fileSet;

CallBackBase *pCallBase,*pCallBack;

void *MainD31Transfer(void *);

void RecvData(THANDLE hTdf, TDF_MSG* pMsgHead);
void RecvSys(THANDLE hTdf, TDF_MSG* pSysMsg);
void DumpScreenFuture(THANDLE hTdf,TDF_FUTURE_DATA* pFuture, int nItems){}
void DumpScreenIndex(TDF_INDEX_DATA* pIndex, int nItems){}
void DumpScreenMarket(TDF_MARKET_DATA* pMarket, int nItems)
{
	pCallBase->OnSubscribe_TdfMkt(pMarket,nItems);
}
void DumpScreenTransaction(TDF_TRANSACTION* pTransaction, int nItems)
{
	pCallBase->OnSubscribe_TdfTra(pTransaction,nItems);
}
void DumpScreenOrder(TDF_ORDER* pOrder, int nItems)
{
	pCallBase->OnSubscribe_TdfOrd(pOrder,nItems);
}
void DumpScreenOrderQueue(TDF_ORDER_QUEUE* pOrderQueue, int nItems)
{
	pCallBase->OnSubscribe_TdfQue(pOrderQueue,nItems);
}
void signalProcess(int signal)
{
	if (signal == SIGUSR1){
		printf("signal SIGUSR1...\n");
		return;
	}
	exit(0);
}
void PrintUsage(char *argv[])
{
	printf("Usage: %s \n", argv[0]);
	printf("   [-c cfg-name ]\n");
	printf("   [-r disp-name ]\n");
	printf("   [-u user-privilege-name ]\n");
	printf("   [-o work-root-name ]\n");
	printf("   [-d work-d31-root ]\n");
	printf("   [-w (2 writetdf,other nowrite) ]\n");
	printf("   [-l max length of mq msg ]\n");
}
int main(int argc, char** argv)
{
	strcpy(sCfgJsonName,	"./tdf_ints.json");
	strcpy(sDispName,	"./disp.json");
	strcpy(sPrivilegeName,	"./user_privilege.json");
	strcpy(sWorkRoot,	"/stock/work");
	strcpy(sWorkD31,	"/data/work");

	for (int c; (c = getopt(argc, argv, "d:c:r:u:o:w:l:?:")) != EOF;){

		switch (c){
		case 'd':
			strcpy(sWorkD31, optarg);
			break;
		case 'c':
			strcpy(sCfgJsonName, optarg);
			break;
		case 'r':
			strcpy(sDispName, optarg);
			break;
		case 'u':
			strcpy(sPrivilegeName, optarg);
			break;
		case 'o':
			strcpy(sWorkRoot, optarg);
			break;
		case 'w':
			iWriteFlag=atoi(optarg);
			if(iWriteFlag!=2) iWriteFlag=0;
			break;
		case 'l':
			SetMaxMqMsgLen(atoi(optarg));
			break;
		case '?':
		default:
			PrintUsage(argv);
			exit(1);
			break;
		}
	}

	if(argc==1){
		PrintUsage(argv);
		exit(1);
	}
	signal(SIGINT, signalProcess);

	//初始化刷新数组，以及刷新文件名的全局变量
	InitUserArray(sDispName,&R);

	//刷新一下参数，避免要求disp先启动，才能启动本程序
	RefreshUserArray(sDispName,&R);
	
	int port,iFailCnt=0;
	string host,id,passwd,strWork;
	boost::property_tree::ptree tRoot;

	strWork=	string(sWorkRoot);
	try{
		boost::property_tree::read_json(sCfgJsonName,tRoot);
		id 	= tRoot.get<string>("id");
		passwd 	= tRoot.get<string>("passwd");
//		passwd	="87613480";
		host 	= tRoot.get<string>("host");
		port 	= tRoot.get<int>("port");

	}
	catch (...) {
		printf("文件 %s 不存在或格式非法.\n",sCfgJsonName);
		exit(1);
	}

	IoService	ios;
	//订阅消息回调类
	pCallBase=new CallBackBase(iWriteFlag,(char*)"",strWork);

	pCallBack=pCallBase;
	pCallBase->SetIoService(&ios);

	//启动处理数据服务
	ios.Start();
	
	TDF_OPEN_SETTING_EXT settings;
	
	memset((void*)&settings, 0, sizeof(settings));
	strcpy(settings.siServer[0].szIp, host.c_str());
	sprintf(settings.siServer[0].szPort, "%d",port);
	strcpy(settings.siServer[0].szUser,id.c_str());
	strcpy(settings.siServer[0].szPwd,passwd.c_str());

	settings.nServerNum = 1; //必须设置： 有效的连接配置个数（当前版本应<=2)

	settings.pfnMsgHandler = RecvData; //设置数据消息回调函数
	settings.pfnSysMsgNotify = RecvSys;//设置系统消息回调函数
	settings.szMarkets = "SZ-2-0;SH-2-0";//需要订阅的市场列表

	settings.szSubScriptions = ""; //"600030.SH"; //600030.SH;104174.SH;103493.SH";	//需要订阅的股票,为空则订阅全市场
	settings.nTime = 0;//请求的时间，格式HHMMSS，为0则请求实时行情，为0xffffffff从头请求
	settings.nTypeFlags = (DATA_TYPE_NONE|DATA_TYPE_TRANSACTION|DATA_TYPE_ORDER|DATA_TYPE_ORDERQUEUE); //请求的品种
	TDF_ERR nErr = TDF_ERR_SUCCESS;
	THANDLE hTDF = NULL;

	//只有连续三次都失败了，才算真失败噢
	while(iFailCnt<3){

		if ((hTDF=TDF_OpenExt(&settings, &nErr))!=NULL) break;
		
		printf("TDF_Open return error: %d ,%d cnt 5s latter try\n",nErr,++iFailCnt);
		sleep(5);
	}

	if(hTDF==NULL){
		printf("TDF_Open return error 3 times\n");
		return -1;
	}
	
	printf(" Open Success!\n");

	pthread_t pthd_d31;
	pthread_attr_t attr_d31;

	//加载d31回放线程
	pthread_attr_init(&attr_d31);
	pthread_attr_setdetachstate(&attr_d31, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&attr_d31, 1024*512);
	pthread_create(&pthd_d31, NULL, MainD31Transfer, NULL);


	printf("-----------------------------1.\n");

	//循环监视disp规则变化，如果变化则通知刷新
	WatchFileCloseWriteAndLock(sDispName);

	printf("-----------------------------2.\n");

	//正常代码不会运行到这里
	while(1){//主线程不能退出
		sleep(1);
	}

	//退出工作线程
	ios.Stop();
	TDF_Close(hTDF);
	return 0;
}

int ReadD31FileAndSend(char sFileName[],long *plCurPos)
{
	static time_t tStartTime=0;

	FILE *fp;
	char sBuffer[10240];
	long lCurPos=*plCurPos,lSize,lCnt,lItemLen;
	struct D31ItemStruct *p=(struct D31ItemStruct*)(sBuffer+sizeof(long long));

	if(tStartTime==0){
		tStartTime=tGetHostTime();
	}

	lSize=lFileSize(sFileName);
	
	if(lSize<=lCurPos) return 0;

	lItemLen=sizeof(struct D31ItemStruct)+sizeof(long long);
	
	lCnt=(lSize-lCurPos)/lItemLen;
	
	if(lCnt==0) return 0;


	if((fp=fopen(sFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sFileName);
		return -1;
	}

	if(fseek(fp,lCurPos,SEEK_SET)<0){
		printf("error fseek file=%s,pos=%ld.\n",sFileName,lCurPos);
		return -1;
	}

	while(lCnt>0){
		if(fread((void*)sBuffer,lItemLen,1,fp)!=1){
			printf("error end of file break.\n");
			return -1;
		}
		//只取时间时间大于启动时间之前1分钟的数据
		if(p->nTradeTime>(tStartTime-60)){

			UTIL_Time stTime;
			PUTIL_GetLocalTime(&stTime);
			long long lCurTime=PUTIL_SystemTimeToDateTime(&stTime);

			//接收到数据后，先放入本地队列，等待数据处理接口处理
			MySubData *d = new MySubData;
			d->nItemCnt=1;
			d->cur_time = lCurTime;
			d->data.assign((const char*)p, sizeof(struct D31ItemStruct));

			TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_D31Item,pCallBack, d)));

			pCallBack->m_ios->Post(task);

		}
		
		lCnt--;
		lCurPos+=lItemLen;
	}
	
	*plCurPos=lCurPos;
	
	fclose(fp);

	return 0;
}

void *MainD31Transfer(void *)
{
	long lCurPos=0;
	char sHostTime[15],sInFileName[1024];
	
	GetHostTime(sHostTime);
	sHostTime[8]=0;

	sprintf(sInFileName,"%s/d31_t3_%s.dat",sWorkD31,sHostTime);
	
	while(1){
		ReadD31FileAndSend(sInFileName,&lCurPos);
		
		//每10毫秒刷一次
		usleep(10*1000);
	}

	return NULL;
}

#define GETRECORD(pBase, TYPE, nIndex) ((TYPE*)((char*)(pBase) + sizeof(TYPE)*(nIndex)))
#define PRINTNUM  1
//static int recordNum = 5;

void RecvData(THANDLE hTdf, TDF_MSG* pMsgHead)
{

	if (!pMsgHead->pData) return;

	unsigned int nItemCount = pMsgHead->pAppHead->nItemCount;
	if (!nItemCount) return;


	static int iCount=0,iPreCnt=0;
	
	iCount++;
//printf("This thread's id is %u \n", (unsigned int)pthread_self());
	if((iCount-iPreCnt)>=MY_INFO_CNT){
		iPreCnt=iCount;
		printf("--------------------------------------receive count=%d,pid=%d tid=%u.\n",
			iCount,getpid(),(unsigned int)pthread_self());
	}

	switch(pMsgHead->nDataType)
	{
	case MSG_DATA_MARKET:
		DumpScreenMarket((TDF_MARKET_DATA*)pMsgHead->pData, nItemCount);
	break;
	case MSG_DATA_FUTURE:
		DumpScreenFuture(hTdf,(TDF_FUTURE_DATA*)pMsgHead->pData, nItemCount);
	break;

	case MSG_DATA_INDEX:
		DumpScreenIndex((TDF_INDEX_DATA*)pMsgHead->pData, nItemCount);
	break;
	case MSG_DATA_TRANSACTION:
		DumpScreenTransaction((TDF_TRANSACTION*)pMsgHead->pData, nItemCount);
	break;
	case MSG_DATA_ORDERQUEUE:
		DumpScreenOrderQueue((TDF_ORDER_QUEUE*)pMsgHead->pData, nItemCount);
	break;
	case MSG_DATA_ORDER:
		DumpScreenOrder((TDF_ORDER*)pMsgHead->pData, nItemCount);
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
			printf("connect: %s:%s user:%s, password:%s suc!\n", 
			pConnResult->szIp, pConnResult->szPort, pConnResult->szUser, pConnResult->szPwd);
		else
			printf("connect: %s:%s user:%s, password:%s fail!\n", 
			pConnResult->szIp, pConnResult->szPort, pConnResult->szUser, pConnResult->szPwd);
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
				printf("market:%s, codeCount:%d, codeDate:%d\n", 
				pCodeResult->szMarket[i], pCodeResult->nCodeCount[i], pCodeResult->nCodeDate[i]);

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
			printf("MSG_SYS_QUOTATIONDATE_CHANGE: market:%s, nOldDate:%d, nNewDate:%d\n",
				pChange->szMarket, pChange->nOldDate, pChange->nNewDate);

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

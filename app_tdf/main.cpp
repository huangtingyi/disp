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


using namespace std;

#include "callsupp.h"


typedef struct _filename_set_
{	
	string tdfmktName;
	string tdfqueName;
	string tdftraName;
	string tdfordName;
	
	FILE *fpTdfMkt;
	FILE *fpTdfQue;
	FILE *fpTdfTra;
	FILE *fpTdfOrd;

} FileNameSet;

typedef struct _my_sub_data_
{
        int nItemCnt;
        long long       cur_time;       ///< 接收到数据时间，精确至毫秒，格式为：DDHHMMSSmmm
        std::string     data;           ///< 数据内容
}MySubData;

class CallBackBase
{
public:
	CallBackBase(int iWriteFlag,string& strWork);
	virtual ~CallBackBase();

	void SetIoService(IoService *ios);
	
	int OpenFileSet(int iWriteFlag,string& strWork);
	void CloseFileSet();

	
	void OnSubscribe_TdfMkt(TDF_MARKET_DATA *p,int nItemCnt);
	void OnSubscribe_TdfTra(TDF_TRANSACTION *p,int nItemCnt);
	void OnSubscribe_TdfOrd(TDF_ORDER *p,int nItemCnt);
	void OnSubscribe_TdfQue(TDF_ORDER_QUEUE *p,int nItemCnt);
public:
	/// 处理订阅数据
	void Deal_Message_TdfMkt(MySubData *d);
	void Deal_Message_TdfTra(MySubData *d);
	void Deal_Message_TdfOrd(MySubData *d);
	void Deal_Message_TdfQue(MySubData *d);


	IoService	*m_ios;
private:
	FileNameSet m_fileSet;
	int	m_iWriteFlag;
};

int CallBackBase::OpenFileSet(int iWriteFlag,string& strWork)
{
	char sHostTime[15];
	
	GetHostTime(sHostTime);
	sHostTime[8]=0;
	
//	memset((void*)&m_fileSet,0,sizeof(FileNameSet));
	
	m_fileSet.fpTdfMkt=m_fileSet.fpTdfQue=m_fileSet.fpTdfTra=m_fileSet.fpTdfOrd=NULL;
	
	if(iWriteFlag==2){
		m_fileSet.tdfmktName=	strWork+"/tdf_mk_"+string(sHostTime)+".dat";
		m_fileSet.tdfqueName=	strWork+"/tdf_qu_"+string(sHostTime)+".dat";
		m_fileSet.tdftraName=	strWork+"/tdf_tr_"+string(sHostTime)+".dat";
		m_fileSet.tdfordName=	strWork+"/tdf_or_"+string(sHostTime)+".dat"; 

		if((m_fileSet.fpTdfMkt=	fopen(m_fileSet.tdfmktName.c_str(),"ab+"))==NULL) return -1;
		if((m_fileSet.fpTdfQue=	fopen(m_fileSet.tdfqueName.c_str(),"ab+"))==NULL) return -1;
		if((m_fileSet.fpTdfTra=	fopen(m_fileSet.tdftraName.c_str(),"ab+"))==NULL) return -1;
		if((m_fileSet.fpTdfOrd=	fopen(m_fileSet.tdfordName.c_str(),"ab+"))==NULL) return -1;
	}
	
	m_iWriteFlag=iWriteFlag;

	return 0;
}

void CallBackBase::CloseFileSet()
{
	if(m_fileSet.fpTdfMkt!=NULL) fclose(m_fileSet.fpTdfMkt);
	if(m_fileSet.fpTdfQue!=NULL) fclose(m_fileSet.fpTdfQue);
	if(m_fileSet.fpTdfTra!=NULL) fclose(m_fileSet.fpTdfTra);
	if(m_fileSet.fpTdfOrd!=NULL) fclose(m_fileSet.fpTdfOrd);
}

CallBackBase::CallBackBase(int iWriteFlag,string& strWork)
{
	if(OpenFileSet(iWriteFlag,strWork)<0){
		printf("打开目录 %s 下写文件失败.\n",strWork.c_str());
		exit(1);
	}
}
CallBackBase::~CallBackBase(void)
{
	CloseFileSet();
}
void CallBackBase::SetIoService(IoService *ios)
{
	m_ios = ios;
}
void CallBackBase::OnSubscribe_TdfMkt(TDF_MARKET_DATA *p,int nItemCnt)
{
	/**
	警告：请勿在回调函数接口内执行耗时操作
	*/
	if ( 0 == m_ios ){
		printf("IoService object is null");
		return ;
	}

	UTIL_Time stTime;
	PUTIL_GetLocalTime(&stTime);

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	MySubData *d = new MySubData;
	d->nItemCnt = nItemCnt;
	d->cur_time = PUTIL_SystemTimeToDateTime(&stTime);

	d->data.assign((const char*)p, sizeof(TDF_MARKET_DATA)*nItemCnt);

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_TdfMkt, this, d)));
	m_ios->Post(task);
}

void CallBackBase::OnSubscribe_TdfTra(TDF_TRANSACTION *p,int nItemCnt)
{
	/**
	警告：请勿在回调函数接口内执行耗时操作
	*/
	if ( 0 == m_ios ){
		printf("IoService object is null");
		return ;
	}

	UTIL_Time stTime;
	PUTIL_GetLocalTime(&stTime);

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	MySubData *d = new MySubData;
	d->nItemCnt = nItemCnt;
	d->cur_time = PUTIL_SystemTimeToDateTime(&stTime);

	d->data.assign((const char*)p, sizeof(TDF_TRANSACTION)*nItemCnt);

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_TdfTra, this, d)));
	m_ios->Post(task);
}
void CallBackBase::OnSubscribe_TdfOrd(TDF_ORDER *p,int nItemCnt)
{
	/**
	警告：请勿在回调函数接口内执行耗时操作
	*/
	if ( 0 == m_ios ){
		printf("IoService object is null");
		return ;
	}

	UTIL_Time stTime;
	PUTIL_GetLocalTime(&stTime);

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	MySubData *d = new MySubData;
	d->nItemCnt = nItemCnt;
	d->cur_time = PUTIL_SystemTimeToDateTime(&stTime);

	d->data.assign((const char*)p, sizeof(TDF_ORDER)*nItemCnt);

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_TdfOrd, this, d)));
	m_ios->Post(task);
}
void CallBackBase::OnSubscribe_TdfQue(TDF_ORDER_QUEUE *p,int nItemCnt)
{
	/**
	警告：请勿在回调函数接口内执行耗时操作
	*/
	if ( 0 == m_ios ){
		printf("IoService object is null");
		return ;
	}

	UTIL_Time stTime;
	PUTIL_GetLocalTime(&stTime);

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	MySubData *d = new MySubData;
	d->nItemCnt = nItemCnt;
	d->cur_time = PUTIL_SystemTimeToDateTime(&stTime);

	d->data.assign((const char*)p, sizeof(TDF_ORDER_QUEUE)*nItemCnt);

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_TdfQue, this, d)));
	m_ios->Post(task);
}

void CallBackBase::Deal_Message_TdfMkt(MySubData *d)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<MySubData> mysubdataptr(d);

	int i,nItemCnt=d->nItemCnt;
	TDF_MARKET_DATA *pHead = (TDF_MARKET_DATA*)&d->data[0],*p;
		
	MktData 	md;
	string strMd;

	for(i=0;i<nItemCnt;i++){
	
		p=pHead+i;

		if(m_iWriteFlag==2){

			fwrite((const void*)&(d->cur_time),sizeof(d->cur_time),1,m_fileSet.fpTdfMkt);
			fwrite((const void*)p,sizeof(TDF_MARKET_DATA),1,m_fileSet.fpTdfMkt);

			fflush(m_fileSet.fpTdfMkt);
		}

		//中午休市期间，行情数据就不要了
		if((p->nTime>113000000&&p->nTime<130000000)||p->nTime>150000000) continue;
	
		TDF_MARKET_DATA2MktData(md,p[0]);


		md.SerializeToString(&strMd);

		int iStockCode=atoi(p->szCode);

		//校验代码合法性
		if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){
			SendMsg2Cli(iStockCode,'M',strMd);
		}
	}
}
void CallBackBase::Deal_Message_TdfTra(MySubData *d)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<MySubData> mysubdataptr(d);

	int i,nItemCnt=d->nItemCnt;
	TDF_TRANSACTION *pHead = (TDF_TRANSACTION*)&d->data[0],*p;

	Transaction 	tr;
	string strTr;

	for(i=0;i<nItemCnt;i++){
	
		p=pHead+i;

		if(m_iWriteFlag==2){

			fwrite((const void*)&(d->cur_time),sizeof(d->cur_time),1,m_fileSet.fpTdfTra);
			fwrite((const void*)p,sizeof(TDF_TRANSACTION),1,m_fileSet.fpTdfTra);

			fflush(m_fileSet.fpTdfTra);
		}
	
		TDF_TRANSACTION2Transaction(tr,p[0]);


		tr.SerializeToString(&strTr);

		int iStockCode=atoi(p->szCode);

		//校验代码合法性
		if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){
			SendMsg2Cli(iStockCode,'T',strTr);
		}
	}
}

void CallBackBase::Deal_Message_TdfOrd(MySubData *d)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<MySubData> mysubdataptr(d);

	int i,nItemCnt=d->nItemCnt;
	TDF_ORDER *pHead = (TDF_ORDER*)&d->data[0],*p;

	Order 	od;
	string strOd;

	for(i=0;i<nItemCnt;i++){
	
		p=pHead+i;

		if(m_iWriteFlag==2){

			fwrite((const void*)&(d->cur_time),sizeof(d->cur_time),1,m_fileSet.fpTdfOrd);
			fwrite((const void*)p,sizeof(TDF_ORDER),1,m_fileSet.fpTdfOrd);

			fflush(m_fileSet.fpTdfOrd);
		}
	
		TDF_ORDER2Order(od,p[0]);

		od.SerializeToString(&strOd);

		int iStockCode=atoi(p->szCode);

		//校验代码合法性
		if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){
			SendMsg2Cli(iStockCode,'O',strOd);
		}
	}
}
void CallBackBase::Deal_Message_TdfQue(MySubData *d)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<MySubData> mysubdataptr(d);

	int i,nItemCnt=d->nItemCnt;
	TDF_ORDER_QUEUE *pHead = (TDF_ORDER_QUEUE*)&d->data[0],*p;

	Order_queue 	oq;
	string strOq;

	for(i=0;i<nItemCnt;i++){
	
		p=pHead+i;

		if(m_iWriteFlag==2){

			fwrite((const void*)&(d->cur_time),sizeof(d->cur_time),1,m_fileSet.fpTdfQue);
			fwrite((const void*)p,sizeof(TDF_ORDER_QUEUE),1,m_fileSet.fpTdfQue);

			fflush(m_fileSet.fpTdfQue);
		}

		TDF_ORDER_QUEUE2Order_queue(oq,p[0]);

		oq.SerializeToString(&strOq);

		int iStockCode=atoi(p->szCode);

		//校验代码合法性
		if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){
			SendMsg2Cli(iStockCode,'Q',strOq);
		}
	}
}

#define ELEM_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))
#define SAFE_STR(str) ((str)?(str):"")
#define SAFE_CHAR(ch) ((ch) ? (ch) : ' ')
	
int MY_INFO_CNT=3000;

int iDebugFlag=0,iWriteFlag=0;
char sCfgJsonName[1024],sDispName[1024],sPrivilegeName[1024],sWorkRoot[1024];

FileNameSet m_m_fileSet;

CallBackBase *pCallBase;


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

int main(int argc, char** argv)
{


	strcpy(sCfgJsonName,	"./tdf_ints.json");
	strcpy(sDispName,	"./disp.json");
	strcpy(sPrivilegeName,	"./user_privilege.json");
	strcpy(sWorkRoot,	"/stock/work");

	for (int c; (c = getopt(argc, argv, "d:c:r:u:o:w:?:")) != EOF;){

		switch (c){
		case 'd':
			iDebugFlag = atoi(optarg);
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
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-c cfg-name ]\n");
			printf("   [-r disp-name ]\n");
			printf("   [-u user-privilege-name ]\n");
			printf("   [-o work-root-name ]\n");
			printf("   [-d DebugFlag ]\n");
			printf("   [-w (2 writetdf,other nowrite) ]\n");
			exit(1);
			break;
		}
	}

	signal(SIGINT, signalProcess);

	//初始化刷新数组，以及刷新文件名的全局变量
	InitUserArray(sDispName,&R);

	//刷新一下参数，避免要求disp先启动，才能启动本程序
	RefreshUserArray(sDispName,&R);
	
	int port;
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
	pCallBase=new CallBackBase(iWriteFlag,strWork);

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

	hTDF = TDF_OpenExt(&settings, &nErr);


	if (hTDF==NULL){
		printf("TDF_Open return error: %d\n", nErr);
	}else{
		printf(" Open Success!\n");
	}
	
	
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


#define GETRECORD(pBase, TYPE, nIndex) ((TYPE*)((char*)(pBase) + sizeof(TYPE)*(nIndex)))
#define PRINTNUM  1
static int recordNum = 5;

void RecvData(THANDLE hTdf, TDF_MSG* pMsgHead)
{

	if (!pMsgHead->pData) return;

	unsigned int nItemCount = pMsgHead->pAppHead->nItemCount;
	if (!nItemCount) return;


	static int iCount=0,iPreCnt=0;
	
	iCount++;

	if((iCount-iPreCnt)>MY_INFO_CNT){
		iPreCnt=iCount;
		printf("--------------------------------------receive count=%d,pid=%d.\n",iCount,getpid());
	}


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
//		TDF_ORDER_QUEUE* pLastOrderQueue = GETRECORD(pMsgHead->pData,TDF_ORDER_QUEUE, nItemCount-1);
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

//////////////////////////////////////////////////////////////////////////
/// @brief    基础API
///           实现不同方式的定阅和取消订阅及快照查询
//////////////////////////////////////////////////////////////////////////
#include "GTAQTSInterfaceBase.h"
#include "CallBackBase.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <boost/property_tree/json_parser.hpp>
#include "IoService.h"
#include "TinyThread.h"
#ifdef _WIN32
#include <windows.h>
#define  SLEEP(t)  Sleep((t)*1000)
#else
#include <unistd.h>
#define  SLEEP(t)  sleep(t)
#endif

#include "callsupp.h"

#include "gta_supp.h"

#include "d31_item.h"
#include "public.h"

#include "wwtiny.h"

CallBackBase *pCallBack;

int iDebugFlag=0,iWriteFlag=0;
char sCfgJsonName[1024],sDispName[1024],sPrivilegeName[1024],sWorkRoot[1024],sWorkD31[1024];

void *MainD31Transfer(void *);

void signalProcess(int signal)
{
	if (signal == SIGUSR1){
		printf("signal SIGUSR1...\n");
		return;
	}
	exit(0);
}

int main(int argc, char *argv[])
{

	strcpy(sCfgJsonName,	"./gta_ints.json");
	strcpy(sDispName,	"./disp.json");
	strcpy(sPrivilegeName,	"./user_privilege.json");
	strcpy(sWorkRoot,	"/stock/work");

	for (int c; (c = getopt(argc, argv, "d:c:r:u:o:w:?:")) != EOF;){

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
			if(iWriteFlag!=1&&iWriteFlag!=2&&iWriteFlag!=3) iWriteFlag=0;
			break;
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-c cfg-name ]\n");
			printf("   [-r disp-name ]\n");
			printf("   [-u user-privilege-name ]\n");
			printf("   [-o work-root-name ]\n");
			printf("   [-d work-d31-home ]\n");
			printf("   [-w (1,writegta,2 writetdf,3,write gta&tdf, other nowrite) ]\n");
			exit(1);
			break;
		}
	}

	signal(SIGINT, signalProcess);

	//初始化刷新数组，以及刷新文件名的全局变量
	InitUserArray(sDispName,&R);

	//刷新一下参数，避免要求disp先启动，才能启动本程序
	RefreshUserArray(sDispName,&R);

	uint16_t port;
	string host,id,passwd,strWork;
	boost::property_tree::ptree tRoot,t;

	strWork=	string(sWorkRoot);
	try{
		boost::property_tree::read_json(sCfgJsonName,tRoot);
		t 	= tRoot.get_child("gta_server");
		id 	= tRoot.get<string>("id");
		passwd 	= tRoot.get<string>("passwd");
	}
	catch (...) {
		printf("文件 %s 不存在或格式非法.\n",sCfgJsonName);
		exit(1);
	}

	IoService	ios;
	//订阅消息回调类
	CallBackBase CallbackBase(iWriteFlag,(char*)"",strWork);
	CallbackBase.SetIoService(&ios);
	
	pCallBack=&CallbackBase;

	//启动处理数据服务
	ios.Start();

	//创建基础API对象
	//详见《国泰安实时行情系统V2.X 用户手册》4.2.1.1 创建实例CreateInstance 章节
	IGTAQTSApiBase* pApiBase = IGTAQTSApiBase::CreateInstance(CallbackBase);
	// 注册FENS地址,如果不注册会在数据高可用方面，会大打折扣。
	for (auto it = t.begin(); it != t.end(); ++it) {
		try {
			host = it->second.get<string>("host");
			port = it->second.get<uint16_t>("port");

			pApiBase->RegisterService(host.c_str(), port);
		}
		catch (...) {
			break;
		}
	}
	//设置超时时间
	pApiBase->SetTimeout(30);

	do{
		//通过用户名与密码向服务器登陆
		//详见《国泰安实时行情系统V2.X 用户手册》4.2.1.5 用户认证Login 章节
		RetCode  ret = pApiBase->LoginX(id.c_str(), passwd.c_str(), "NetType=0");
		if (Ret_Success != ret){
			printf("Login error:%d\n", ret);
			break;
		}

		//****************************** 获取证券代码列表及权限列表****************************************
		CDataBuffer<StockSymbol> StockList1;

		// 获取上交所和深交所代码列表，其中SSE表示上交所，SZSE表示深交所，CFFEX表示中金所
		//详见《国泰安实时行情系统V2.X 用户手册》4.2.1.11 获取代码列表GetStockList 章节
		ret = pApiBase->GetStockList((char*)"sse,szse", StockList1);
		if (Ret_Success != ret){
			printf("GetStockList(sse,szse) error:%d\n", ret);
			break;
		}

		StockSymbol* pStock = StockList1;
		int sz = StockList1.Size();
		char sShStr[40960],sSzStr[40960];
		
		GetStockStrAll(pStock,sz,sShStr,sSzStr);


/*		VectorStockCodeSH vSH;
		VectorStockCodeSZ vSZ;

		for (int i = 0; i < sz; ++i) {
			if (!(vSH.push(pStock[i].Symbol))) {
				vSZ.push(pStock[i].Symbol);
			}
		}
		printf("\n");
		
*/
		CDataBuffer<MsgType> DataTypeList;
		// 获取权限列表
		//详见《国泰安实时行情系统V2.X 用户手册》4.1.1.7 获取权限列表GetDataTypeList 章节
		ret = pApiBase->GetDataTypeList(DataTypeList);
		if (Ret_Success != ret){
			printf("GetDataTypeList(sse) error:%d\n", ret);
			break;
		}

		MsgType* pMsg = DataTypeList;
		int Count = DataTypeList.Size();
		printf("MsgType Count = %d, List:", Count);
		for (int i = 0; i < Count; ++i)
		{
			printf("Ox%08x, ", pMsg[i]);
		}
		printf("\n");

		//************************************订阅行情数据***********************************************

		// 按代码订阅深交所实时行情数据
		//详见《国泰安实时行情系统V2.X 用户手册》4.1.1.8 订阅实时数据Subscribe 章节
//		string strCodesSH,strCodesSZ;
//		vSH.strForSub(strCodesSH);
//		vSZ.strForSub(strCodesSZ);
		
		//初始化涨停跌停数组
		InitLimitArray();
		//获取静态上海数据,获取涨停跌停数值
		CDataBuffer<SSEL2_Static> Snap_Quotation;
		//ret = pApiBase->QuerySnap_SSEL2_Static((char*)(strCodesSH.c_str()), Snap_Quotation);
		ret = pApiBase->QuerySnap_SSEL2_Static(sShStr, Snap_Quotation);
		if (Ret_Success != ret) {
			return false;
		}
		// 获取全部快照
		sz = Snap_Quotation.Size();
		for (int i = 0; i < sz; ++i) {
			SSEL2_Static& SS = Snap_Quotation[i];
			int iStockCode=atoi(SS.Symbol);
			
			if(iStockCode>=0&&iStockCode<MAX_STOCK_CODE){
				LIMIT[iStockCode].WarrantDownLimit=	SS.PriceDownLimit;
				LIMIT[iStockCode].WarrantUpLimit=	SS.PriceUpLimit;
			}
			//g_LimitPriceMgr.update(SS.Symbol, SS.PriceUpLimit, SS.PriceDownLimit);
		}

		//上海L2集合竞价
		//ret = pApiBase->Subscribe(Msg_SSEL2_Auction, (char*)(strCodesSH.c_str()));
		ret = pApiBase->Subscribe(Msg_SSEL2_Auction, sShStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SSEL2_Auction code=%d\n",ret);
			break;
		}

		//上海L2实时行情
		//ret = pApiBase->Subscribe(Msg_SSEL2_Quotation, (char*)(strCodesSH.c_str()));
		ret = pApiBase->Subscribe(Msg_SSEL2_Quotation, sShStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SSEL2_Quotation code=%d\n",ret);
			break;
		}
		//上海L2实时交易
		//ret = pApiBase->Subscribe(Msg_SSEL2_Transaction, (char*)(strCodesSH.c_str()));
		ret = pApiBase->Subscribe(Msg_SSEL2_Transaction, sShStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SSEL2_Transaction code=%d\n",ret);
			break;
		}

		//深圳L2实时行情
		//ret = pApiBase->Subscribe(Msg_SZSEL2_Quotation, (char*)(strCodesSZ.c_str()));
		ret = pApiBase->Subscribe(Msg_SZSEL2_Quotation, sSzStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SZSEL2_Quotation code=%d\n",ret);
			break;
		}
		//深圳L2实时交易
		//ret = pApiBase->Subscribe(Msg_SZSEL2_Transaction, (char*)(strCodesSZ.c_str()));
		ret = pApiBase->Subscribe(Msg_SZSEL2_Transaction, sSzStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SZSEL2_Transaction code=%d\n",ret);
			break;
		}
		//深圳L2逐笔委托
		//ret = pApiBase->Subscribe(Msg_SZSEL2_Order, (char*)(strCodesSZ.c_str()));
		ret = pApiBase->Subscribe(Msg_SZSEL2_Order, sSzStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SZSEL2_Order code=%d\n",ret);
			break;
		}

	}
	while (false);


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

	// 当不再使用API时，需要调用此接口释放内部资源，否则会引起内存泄漏及不可预知问题
	//详见《国泰安实时行情系统V2.X 用户手册》4.2.1.2 释放实例ReleaseInstance 章节
	IGTAQTSApiBase::ReleaseInstance(pApiBase);

	return 0;
}

int ReadD31FileAndSend(char sFileName[],long *plCurPos)
{
	FILE *fp;
	char sBuffer[10240];
	long lCurPos=*plCurPos,lSize,lCnt,lItemLen;
	struct D31ItemStruct *p=(struct D31ItemStruct*)(sBuffer+sizeof(long long));

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
		{

			UTIL_Time stTime;
			PUTIL_GetLocalTime(&stTime);
			long long lCurTime=PUTIL_SystemTimeToDateTime(&stTime);

			//接收到数据后，先放入本地队列，等待数据处理接口处理
			SubData *subdata = new SubData;
			subdata->cur_time = lCurTime;
			subdata->data.assign((const char*)p, sizeof(struct D31ItemStruct));

			TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_D31Item,pCallBack, subdata)));

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

	sprintf(sInFileName,"%s/d31_g3_%s.dat",sWorkD31,sHostTime);
	
	while(1){
		ReadD31FileAndSend(sInFileName,&lCurPos);
		
		//每10毫秒刷一次
		usleep(10*1000);
	}

	return NULL;
}
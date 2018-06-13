#include "CallBackBase.h"
#include <stdio.h>
#include <atomic>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/inotify.h>
//#include <boost/property_tree/json_parser.hpp>

#include "public.h"
#include "tdf_supp.h"
//#include "gta_supp.h"


#include "mktdata.pb.h"  
#include "MessageQueue.h"

#include "TDFAPIStruct.h"

#include "wwtiny.h"
#include "callsupp.h"

#define AP_SHJJ_CLOSE_TIME	92400000
#define AM_MARKET_OPEN_TIME	93000000
#define AM_MARKET_CLOSE_TIME	113100000
#define PM_MARKET_OPEN_TIME	130000000
#define PM_MARKET_CLOSE_TIME	151000000
#define PM_MARKET_PRE_CLOSE_TIME 145800000

int CallBackBase::OpenFileSet(int iWriteFlag,char sDataDate[],string& strWork)
{
	char sHostTime[15];
	
	if(strlen(sDataDate)==0)
		GetHostTime(sHostTime);
	else	strcpy(sHostTime,sDataDate);
	
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

CallBackBase::CallBackBase(int iWriteFlag,char sDataDate[],string& strWork)
{
	if(OpenFileSet(iWriteFlag,sDataDate,strWork)<0){
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

		//不是特定的股票代码就直接pass了
		if(ValidStockCode(p->szWindCode)==false) continue;
			

		//中午休市期间，行情数据就不要了，做得和GTA一样的
		if((p->nTime>AM_MARKET_CLOSE_TIME&&p->nTime<PM_MARKET_OPEN_TIME)||
			p->nTime>PM_MARKET_CLOSE_TIME) continue;
	
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
	
		//不是特定的股票代码就直接pass了
		if(ValidStockCode(p->szWindCode)==false) continue;

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
	
		//不是特定的股票代码就直接pass了
		if(ValidStockCode(p->szWindCode)==false) continue;

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

		//不是特定的股票代码就直接pass了
		if(ValidStockCode(p->szWindCode)==false) continue;

		TDF_ORDER_QUEUE2Order_queue(oq,p[0]);

		oq.SerializeToString(&strOq);

		int iStockCode=atoi(p->szCode);

		//校验代码合法性
		if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){
			SendMsg2Cli(iStockCode,'Q',strOq);
		}
	}
}

void CallBackBase::Deal_Message_D31Item(MySubData *d)
{
	static int iMyCnt=0;
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<MySubData> mysubdataptr(d);

	struct D31ItemStruct *p = (struct D31ItemStruct *)&d->data[0];

	D31Item di;
	string strDi;

	D31_ITEM2D31Item(di,p[0]);

	di.SerializeToString(&strDi);

	int iStockCode=p->nStockCode;
	
		//校验代码合法性
	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){

//		printf("stock_code=%d,time=%d.\n",iStockCode,p->nTradeTime);

		iMyCnt++;

//		if(iStockCode==673&&p->nTradeTime==1527039000){
//			printf("stock_code=%d,time=%d cnt=%d.\n",iStockCode,p->nTradeTime,iMyCnt);
//		}		
		//设置callsupp.cpp中定义的全局变量，供D31TradeTimeValid使用
		nD31TradeTime=p->nTradeTime;
		SendMsg2Cli(iStockCode,'D',strDi);
	}
}

#include "CallBackBase.h"
#include <stdio.h>
#include <atomic>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#include <boost/property_tree/json_parser.hpp>

#include "public.h"
#include "../GTA2TDF/GTA2TDF.h"
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



CallBackBase::CallBackBase(int iWriteFlag,char sDataDate[],string& strWork)
{
	m_ios = 0;
	char sHostTime[15];
	
	if(strlen(sDataDate)==0)
		GetHostTime(sHostTime);
	else	strcpy(sHostTime,sDataDate);
	
	sHostTime[8]=0;
	
	if(iWriteFlag&1){
		m_fileSet.gtaqhName=	strWork+"/gta_qh_"+string(sHostTime)+".dat";
		m_fileSet.gtaahName=	strWork+"/gta_ah_"+string(sHostTime)+".dat";
		m_fileSet.gtathName=	strWork+"/gta_th_"+string(sHostTime)+".dat";
		m_fileSet.gtaqzName=	strWork+"/gta_qz_"+string(sHostTime)+".dat";
		m_fileSet.gtatzName=	strWork+"/gta_tz_"+string(sHostTime)+".dat";
		m_fileSet.gtaozName=	strWork+"/gta_oz_"+string(sHostTime)+".dat";
		
		m_fileSet.fpGtaQh=	fopen(m_fileSet.gtaqhName.c_str(),"ab+");
		m_fileSet.fpGtaAh=	fopen(m_fileSet.gtaahName.c_str(),"ab+");
		m_fileSet.fpGtaTh=	fopen(m_fileSet.gtathName.c_str(),"ab+");
		m_fileSet.fpGtaQz=	fopen(m_fileSet.gtaqzName.c_str(),"ab+");
		m_fileSet.fpGtaTz=	fopen(m_fileSet.gtatzName.c_str(),"ab+");
		m_fileSet.fpGtaOz=	fopen(m_fileSet.gtaozName.c_str(),"ab+");

	}
	if(iWriteFlag&2){
		m_fileSet.tdfmktName=	strWork+"/tdf_mkt_"+string(sHostTime)+".dat";
		m_fileSet.tdfqueName=	strWork+"/tdf_que_"+string(sHostTime)+".dat";
		m_fileSet.tdftraName=	strWork+"/tdf_tra_"+string(sHostTime)+".dat";
		m_fileSet.tdfordName=	strWork+"/tdf_ord_"+string(sHostTime)+".dat"; 

		m_fileSet.fpTdfMkt=	fopen(m_fileSet.tdfmktName.c_str(),"ab+");
		m_fileSet.fpTdfQue=	fopen(m_fileSet.tdfqueName.c_str(),"ab+");
		m_fileSet.fpTdfTra=	fopen(m_fileSet.tdftraName.c_str(),"ab+");
		m_fileSet.fpTdfOrd=	fopen(m_fileSet.tdfordName.c_str(),"ab+");	
	}
	m_iWriteFlag=iWriteFlag;
}


CallBackBase::~CallBackBase(void){}

void CallBackBase::SetIoService(IoService *ios)
{
	m_ios = ios;
}

void CallBackBase::OnLoginState(RetCode errCode)
{
//	BOOST_LOG_SEV(g_lg, info) << __FUNCTION__ << ' ' << errCode;
}

/// 连接状态返回，连接成功/失败
/// @param  msgType     -- 消息类型
/// @param  errCode     -- 失败原因，成功时返回0
///                     详见《国泰安实时行情系统V2.X 用户手册》5.5返回码含义列表RetCode 章节
void CallBackBase::OnConnectionState(MsgType msgType, RetCode errCode)
{
	printf("MsgType:0x%x ConnectionState:%d\n", msgType, errCode);
//	BOOST_LOG_SEV(g_lg, info) << __FUNCTION__ << ' ' << msgType << ' ' << errCode;
}

void CallBackBase::OnSubscribe_SSEL2_Quotation(const SSEL2_Quotation& RealSSEL2Quotation)
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
	
	//必须将数据调整放在这，避免回放程序错误
	int iStockCode=atoi(RealSSEL2Quotation.Symbol);

	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){

		SSEL2_Quotation *p=(SSEL2_Quotation *)&RealSSEL2Quotation;
		p->WarrantDownLimit=	LIMIT[iStockCode].WarrantDownLimit;
		p->WarrantUpLimit=	LIMIT[iStockCode].WarrantUpLimit;
	}

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	SubData *subdata = new SubData;
	subdata->msgType = Msg_SSEL2_Quotation;
	subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
	subdata->data.assign((const char*)&RealSSEL2Quotation, sizeof(RealSSEL2Quotation));

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_SSEL2_Quotation, this, subdata)));
	m_ios->Post(task);
}

/// 上交所L2逐笔成交订阅数据实时回调接口
/// @param  RealSSEL2Transaction    -- 实时数据
void CallBackBase::OnSubscribe_SSEL2_Transaction(const SSEL2_Transaction& RealSSEL2Transaction)
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
	SubData *subdata = new SubData;
	subdata->msgType = Msg_SSEL2_Transaction;
	subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
	subdata->data.assign((const char*)&RealSSEL2Transaction, sizeof(RealSSEL2Transaction));

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_SSEL2_Transaction, this, subdata)));
	m_ios->Post(task);
}

void CallBackBase::OnSubscribe_SSEL2_Auction(const SSEL2_Auction& RealSSEL2Auction)
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
	SubData *subdata = new SubData;
	subdata->msgType = Msg_SSEL2_Auction;
	subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
	subdata->data.assign((const char*)&RealSSEL2Auction, sizeof(RealSSEL2Auction));

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_SSEL2_Auction, this, subdata)));
	m_ios->Post(task);
}

/// 深交所L2实时行情订阅数据实时回调接口
/// @param  RealSZSEL2Quotation -- 实时数据
void CallBackBase::OnSubscribe_SZSEL2_Quotation(const SZSEL2_Quotation& RealSZSEL2Quotation)
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
	SubData *subdata = new SubData;
	subdata->msgType = Msg_SZSEL2_Quotation;
	subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
	subdata->data.assign((const char*)&RealSZSEL2Quotation, sizeof(RealSZSEL2Quotation));

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_SZSEL2_Quotation, this, subdata)));
	m_ios->Post(task);
}

void CallBackBase::OnSubscribe_SZSEL2_Transaction(const SZSEL2_Transaction& RealSZSEL2Transaction)
{
	if (0 == m_ios)	{
		printf("IoService object is null");
		return;
	}

	UTIL_Time stTime;
	PUTIL_GetLocalTime(&stTime);

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	SubData *subdata = new SubData;
	subdata->msgType = Msg_SZSEL2_Transaction;
	subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
	subdata->data.assign((const char*)&RealSZSEL2Transaction, sizeof(RealSZSEL2Transaction));

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_SZSEL2_Transaction, this, subdata)));
	m_ios->Post(task);
}

/*virtual*/
void CallBackBase::OnSubscribe_SZSEL2_Order(const SZSEL2_Order& RealSZSEL2Order)
{
	if (0 == m_ios) {
		printf("IoService object is null");
		return;
	}

	UTIL_Time stTime;
	PUTIL_GetLocalTime(&stTime);

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	SubData *subdata = new SubData;
	subdata->msgType = Msg_SZSEL2_Order;
	subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
	subdata->data.assign((const char*)&RealSZSEL2Order, sizeof(RealSZSEL2Order));

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_SZSEL2_Order, this, subdata)));
	m_ios->Post(task);
}



int max_trans_cnt=1,iTotalCnt=0;

/// 处理上交所L2实时行情快照订阅数据
void CallBackBase::Deal_Message_SSEL2_Quotation(SubData *subdata)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<SubData> subdataptr(subdata);

	SSEL2_Quotation *RealSSEL2Quotation = (SSEL2_Quotation*)&subdata->data[0];
	TDF_MARKET_DATA m;
	TDF_ORDER_QUEUE q[2];
	
	MktData 	md;
	Order_queue 	oq0,oq1;
	string strMd,strOq0,strOq1;

	for(int i=0;i<max_trans_cnt;i++)
	GTA2TDF_SSEL2(RealSSEL2Quotation[0],m, q[0],q[1]);

	if(m_iWriteFlag&1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaQh);
		fwrite((const void*)RealSSEL2Quotation,sizeof(SSEL2_Quotation),1,m_fileSet.fpGtaQh);
		fflush(m_fileSet.fpGtaQh);
	}
	if(m_iWriteFlag&2){

		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfMkt);
		fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,m_fileSet.fpTdfMkt);
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfQue);
		fwrite((const void*)&q[0],sizeof(TDF_ORDER_QUEUE),1,m_fileSet.fpTdfQue);
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfQue);
		fwrite((const void*)&q[1],sizeof(TDF_ORDER_QUEUE),1,m_fileSet.fpTdfQue);
	}

	//中午休市期间以及收盘后的，行情数据就不要了
	//中午休市期间，行情数据就不要了
	if((m.nTime>AM_MARKET_CLOSE_TIME&&m.nTime<PM_MARKET_OPEN_TIME)||
		m.nTime>PM_MARKET_CLOSE_TIME) return;

	if(m.nTime>PM_MARKET_PRE_CLOSE_TIME){
		__int64 nClose=yuan2percentFen(RealSSEL2Quotation->ClosePrice);
		
		if(nClose!=0)m.nMatch=nClose;
	}
	
	TDF_MARKET_DATA2MktData(md,m);

	TDF_ORDER_QUEUE2Order_queue(oq0,q[0]);
	TDF_ORDER_QUEUE2Order_queue(oq1,q[1]);


	md.SerializeToString(&strMd);
	oq0.SerializeToString(&strOq0);
	oq1.SerializeToString(&strOq1);

	int iStockCode=atoi(RealSSEL2Quotation->Symbol);

		//校验代码合法性
	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){
		SendMsg2Cli(iStockCode,'M',strMd);
		SendMsg2Cli(iStockCode,'Q',strOq0);
		SendMsg2Cli(iStockCode,'Q',strOq1);
	}
}

void CallBackBase::Deal_Message_SSEL2_Transaction(SubData *subdata)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<SubData> subdataptr(subdata);

	SSEL2_Transaction *RealSSEL2Transaction = (SSEL2_Transaction*)&subdata->data[0];
	TDF_TRANSACTION t;

	Transaction 	tr;
	string strTr;

	for(int i=0;i<max_trans_cnt;i++)
	GTA2TDF_SSEL2_T(RealSSEL2Transaction[0],t);

	if(m_iWriteFlag&1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaTh);
		fwrite((const void*)RealSSEL2Transaction,sizeof(SSEL2_Transaction),1,m_fileSet.fpGtaTh);
		fflush(m_fileSet.fpGtaTh);
	}
	if(m_iWriteFlag&2){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfTra);
		fwrite((const void*)&t,sizeof(TDF_TRANSACTION),1,m_fileSet.fpTdfTra);
		fflush(m_fileSet.fpTdfTra);
	}

	TDF_TRANSACTION2Transaction(tr,t);

	tr.SerializeToString(&strTr);

	int iStockCode=atoi(RealSSEL2Transaction->Symbol);
		//校验代码合法性
	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){

		SendMsg2Cli(iStockCode,'T',strTr);
	}
}
void CallBackBase::Deal_Message_SSEL2_Auction(SubData *subdata)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<SubData> subdataptr(subdata);

	SSEL2_Auction *RealSSEL2Auction = (SSEL2_Auction*)&subdata->data[0];
	TDF_MARKET_DATA m;
	MktData 	md;
	string strMd;

	for(int i=0;i<max_trans_cnt;i++)
	GTA2TDF_SSEL2_AM(RealSSEL2Auction[0],m);

	if(m_iWriteFlag&1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaAh);
		fwrite((const void*)RealSSEL2Auction,sizeof(SSEL2_Auction),1,m_fileSet.fpGtaAh);
		fflush(m_fileSet.fpGtaAh);
	}
	if(m_iWriteFlag&2){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfMkt);
		fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,m_fileSet.fpTdfMkt);
		fflush(m_fileSet.fpTdfMkt);
	}

	//9点25分后，成交量为0的，上海集合竞价的数据，就不要转为行情了
	if(m.nTime>AP_SHJJ_CLOSE_TIME&&m.iVolume==0) return;

	TDF_MARKET_DATA2MktData(md,m);

	md.SerializeToString(&strMd);

	int iStockCode=atoi(RealSSEL2Auction->Symbol);
		//校验代码合法性
	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){

		SendMsg2Cli(iStockCode,'M',strMd);
	}
}
void CallBackBase::Deal_Message_SZSEL2_Quotation(SubData *subdata)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<SubData> subdataptr(subdata);
	SZSEL2_Quotation *RealSZSEL2Quotation = (SZSEL2_Quotation*)&subdata->data[0];

	TDF_MARKET_DATA m;
	TDF_ORDER_QUEUE q[2];

	MktData 	md;
	Order_queue 	oq0,oq1;
	string strMd,strOq0,strOq1;


	for(int i=0;i<max_trans_cnt;i++)
	GTA2TDF_SZSEL2(RealSZSEL2Quotation[0],m, q[0],q[1]);

	if(m_iWriteFlag&1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaQz);
		fwrite((const void*)RealSZSEL2Quotation,sizeof(SZSEL2_Quotation),1,m_fileSet.fpGtaQz);
		fflush(m_fileSet.fpGtaQz);
	}
	if(m_iWriteFlag&2){

		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfMkt);
		fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,m_fileSet.fpTdfMkt);
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfQue);
		fwrite((const void*)&q[0],sizeof(TDF_ORDER_QUEUE),1,m_fileSet.fpTdfQue);
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfQue);
		fwrite((const void*)&q[1],sizeof(TDF_ORDER_QUEUE),1,m_fileSet.fpTdfQue);
		
		fflush(m_fileSet.fpTdfMkt);
		fflush(m_fileSet.fpTdfQue);
	}

	//中午休市期间，行情数据就不要了
	if((m.nTime>AM_MARKET_CLOSE_TIME&&m.nTime<PM_MARKET_OPEN_TIME)||
		m.nTime>PM_MARKET_CLOSE_TIME) return;

	TDF_MARKET_DATA2MktData(md,m);

	TDF_ORDER_QUEUE2Order_queue(oq0,q[0]);
	TDF_ORDER_QUEUE2Order_queue(oq1,q[1]);

	md.SerializeToString(&strMd);
	oq0.SerializeToString(&strOq0);
	oq1.SerializeToString(&strOq1);

	int iStockCode=atoi(RealSZSEL2Quotation->Symbol);
	//校验代码合法性
	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){
		SendMsg2Cli(iStockCode,'M',strMd);
		SendMsg2Cli(iStockCode,'Q',strOq0);
		SendMsg2Cli(iStockCode,'Q',strOq1);
	}
}

void CallBackBase::Deal_Message_SZSEL2_Transaction(SubData *subdata)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<SubData> subdataptr(subdata);

	SZSEL2_Transaction *RealSZSEL2Transaction = (SZSEL2_Transaction*)&subdata->data[0];

	TDF_TRANSACTION t;
	Transaction 	tr;
	string strTr;

	for(int i=0;i<max_trans_cnt;i++)
	GTA2TDF_SZSEL2_T(RealSZSEL2Transaction[0],t);

	if(m_iWriteFlag&1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaTz);
		fwrite((const void*)RealSZSEL2Transaction,sizeof(SZSEL2_Transaction),1,m_fileSet.fpGtaTz);
		fflush(m_fileSet.fpGtaTz);
	}
	if(m_iWriteFlag&2){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfTra);
		fwrite((const void*)&t,sizeof(TDF_TRANSACTION),1,m_fileSet.fpTdfTra);
		fflush(m_fileSet.fpTdfTra);
	}

	TDF_TRANSACTION2Transaction(tr,t);

	tr.SerializeToString(&strTr);

	int iStockCode=atoi(RealSZSEL2Transaction->Symbol);
		//校验代码合法性
	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){

		SendMsg2Cli(iStockCode,'T',strTr);
	}

	
}
void CallBackBase::Deal_Message_SZSEL2_Order(SubData *subdata)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<SubData> subdataptr(subdata);

	SZSEL2_Order *RealSZSEL2Order = (SZSEL2_Order*)&subdata->data[0];

	TDF_ORDER o;
	Order 	od;
	string strOd;

	for(int i=0;i<max_trans_cnt;i++)
	GTA2TDF_SZSEL2_O(RealSZSEL2Order[0],o);

	if(m_iWriteFlag&1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaOz);
		fwrite((const void*)RealSZSEL2Order,sizeof(SZSEL2_Order),1,m_fileSet.fpGtaOz);
		fflush(m_fileSet.fpGtaOz);
	}
	if(m_iWriteFlag&2){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpTdfOrd);
		fwrite((const void*)&o,sizeof(TDF_ORDER),1,m_fileSet.fpTdfOrd);
		fflush(m_fileSet.fpTdfOrd);
	}

	TDF_ORDER2Order(od,o);

	od.SerializeToString(&strOd);

	int iStockCode=atoi(RealSZSEL2Order->Symbol);
		//校验代码合法性
	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){

		SendMsg2Cli(iStockCode,'O',strOd);
	}
}

void CallBackBase::Deal_Message_D31Item(SubData *subdata)
{
	//智能指针，会自动释放指针指向的对象，释放内存空间
	std::unique_ptr<SubData> subdataptr(subdata);

	struct D31ItemStruct *p = (struct D31ItemStruct *)&subdata->data[0];

	D31Item di;
	string strDi;


	D31_ITEM2D31Item(di,p[0]);

	di.SerializeToString(&strDi);

	int iStockCode=p->nStockCode;
		//校验代码合法性
	if(iStockCode>0&&iStockCode<MAX_STOCK_CODE){

		SendMsg2Cli(iStockCode,'D',strDi);
	}
}


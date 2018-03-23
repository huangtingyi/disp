#include "CallBackBase.h"
#include <stdio.h>
#include <atomic>

#include <boost/property_tree/json_parser.hpp>

#include "../Common/public.h"
#include "../GTA2TDF/GTA2TDF.h"
#include "mktdata.pb.h"  
#include "MessageQueue.h"

//atomic_t VLock=ATOMIC_INIT(0);

int VLock=0;

void LockWorkThread()
{
	//atomic_inc(&VLock);
	VLock=1;
}
void UnLockWorkThread()
{
	//atomic_dec(&VLock);
	VLock=0;
}
int IsWorkThreadLock()
{
	//return atomic_read(&VLock);
	return VLock;
}

void TDF_MARKET_DATA2MktData(MktData &output, const TDF_MARKET_DATA &src)
{
	output.set_szcode(strtoul(src.szCode,nullptr,10));	//600001.SH
	output.set_ntime(src.nTime);			//时间(HHMMSSmmm)
	output.set_nstatus(src.nStatus);		//状态
	output.set_npreclose(uint32_t(src.nPreClose));	//前收盘价
	output.set_nopen(uint32_t(src.nOpen));			//开盘价
	output.set_nhigh(uint32_t(src.nHigh));			//最高价
	output.set_nlow(uint32_t(src.nLow));				//最低价
	output.set_nmatch(uint32_t(src.nMatch));			//最新价
	for (unsigned char i = 0; i < 10; ++i) {
		output.add_naskprice(uint32_t(src.nAskPrice[i]));//申卖价
		output.add_naskvol(src.nAskVol[i]);	//申卖量
		output.add_nbidprice(uint32_t(src.nBidPrice[i]));//申买价
		output.add_nbidvol(src.nBidVol[i]);	//申买量
	}
	output.set_nnumtrades(src.nNumTrades);	//成交笔数
	output.set_ivolume(src.iVolume);		//成交总量
	output.set_iturnover(src.iTurnover);	//成交总金额
	output.set_ntotalbidvol(src.nTotalBidVol);//委托买入总量
	output.set_ntotalaskvol(src.nTotalAskVol);//委托卖出总量
	output.set_nweightedavgbidprice(uint32_t(src.nWeightedAvgBidPrice));//加权平均委买价格
	output.set_nweightedavgaskprice(uint32_t(src.nWeightedAvgAskPrice));//加权平均委卖价格
	output.set_niopv(src.nIOPV);		//IOPV净值估值
	output.set_nyieldtomaturity(src.nYieldToMaturity);//到期收益率
	output.set_nhighlimited(uint32_t(src.nHighLimited));//涨停价
	output.set_nlowlimited(uint32_t(src.nLowLimited));//跌停价
	output.set_nsyl1(src.nSyl1);			//市盈率1
	output.set_nsyl2(src.nSyl2);			//市盈率2
	output.set_nsd2(src.nSD2);				//升跌2（对比上一笔）
}

void TDF_TRANSACTION2Transaction(Transaction &output, const TDF_TRANSACTION &src)
{
	output.set_szcode(strtoul(src.szCode, nullptr, 10));	//600001.SH
	output.set_ntime(src.nTime);			//成交时间(HHMMSSmmm)
	output.set_nindex(src.nIndex);			//成交编号
	output.set_nprice(uint32_t(src.nPrice));//成交价格
	output.set_nvolume(src.nVolume);		//成交数量
	output.set_nturnover(src.nTurnover);	//成交金额
	output.set_nbsflag(src.nBSFlag);		//买卖方向(买：'B', 卖：'S', 不明：' ')
	output.set_chorderkind(src.chOrderKind);//成交类别
	output.set_chfunctioncode(src.chFunctionCode);//成交代码
	output.set_naskorder(src.nAskOrder);	//叫卖方委托序号
	output.set_nbidorder(src.nBidOrder);
}

void TDF_ORDER2Order(Order &output, const TDF_ORDER&src)
{
	output.set_szcode(strtoul(src.szCode, nullptr, 10));	//600001.SH
	output.set_ntime(src.nTime);			//时间(HHMMSSmmm)
	output.set_norder(src.nOrder);		//委托号
	output.set_nprice(uint32_t(src.nPrice));//委托价格
	output.set_nvolume(src.nVolume);		//委托数量
	output.set_chorderkind(src.chOrderKind);//委托类别
	output.set_chfunctioncode(src.chFunctionCode);//委托代码('B','S','C')
	output.set_nbroker(src.nBroker);		//经纪商编码
	output.set_chstatus(src.chStatus);		//委托状态
	output.set_chflag(src.chFlag);
}

void TDF_ORDER_QUEUE2Order_queue(Order_queue &output, const TDF_ORDER_QUEUE&src)
{
	output.set_szcode(strtoul(src.szCode, nullptr, 10));	//600001.SH
	output.set_ntime(src.nTime);			//时间(HHMMSSmmm)
	output.set_nside(src.nSide=='B'||src.nSide=='b');//买卖方向('B':Bid 'A':Ask)
	output.set_nprice(uint32_t(src.nPrice));//委托价格
	output.set_norders(src.nOrders);		//订单数量
	output.set_nabitems(src.nABItems);		//明细个数
	for (int i = 0; i < src.nABItems; ++i) {
		output.add_nabvolume(src.nABVolume[i]);
	}
}

#define MAX_USER_NAME_LEN 64
#define MAX_STOCK_CODE	1000000

struct UserStruct
{
	struct UserStruct *pNext;
	char sUserName[MAX_USER_NAME_LEN];
	int iMqId;
};

char sFlashDispName[1024];
struct UserStruct *AMUSER[MAX_STOCK_CODE],*ATUSER[MAX_STOCK_CODE];
struct UserStruct *AQUSER[MAX_STOCK_CODE],*AOUSER[MAX_STOCK_CODE];
struct UserStruct *PMALL,*PTALL,*PQALL,*POALL;

void InitUserArray(char sDispName[])
{
/*将映射表清空*/
	for(int i=0;i<MAX_STOCK_CODE;i++){
		AMUSER[i]=NULL;
		ATUSER[i]=NULL;
		AQUSER[i]=NULL;
		AOUSER[i]=NULL;
	}
	PMALL=PTALL=PQALL=POALL=NULL;
	strcpy(sFlashDispName,sDispName);
}
void DeleteUserList(struct UserStruct *ptHead)
{
	struct UserStruct *pTemp;
	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;
		delete pTemp;
	}
}
void FlashUserArray(char sDispName[])
{
	int i=0,iSubscribed,iStockCode;

	struct UserStruct **AUSER,*pTemp,**PPALL;

	boost::property_tree::ptree tRoot,tMainRoot,t,t1,t2;
	string user,mqid;

/*读取disp.json文件,先检查一下格式是否完整，不完整就不刷新*/
	try {
		boost::property_tree::read_json(sDispName,tMainRoot);
		tRoot=tMainRoot.get_child("users");
		auto itmp=tRoot.begin();
		
		user = itmp->second.get<string>("user");
		mqid = itmp->second.get<string>("mqid");
		t1 = itmp->second.get_child("subscribed");
		t2 = itmp->second.get_child("subcodes");
	}
	catch (...) {
		printf("catch not full ------------------------.\n");
		return;
	}


/*将映射表清空*/
	for(i=0;i<MAX_STOCK_CODE;i++){
		DeleteUserList(AMUSER[i]);AMUSER[i]=NULL;
		DeleteUserList(ATUSER[i]);ATUSER[i]=NULL;
		DeleteUserList(AQUSER[i]);AQUSER[i]=NULL;
		DeleteUserList(AOUSER[i]);AOUSER[i]=NULL;
	}
	DeleteUserList(PMALL);
	DeleteUserList(PTALL);
	DeleteUserList(PQALL);
	DeleteUserList(POALL);
	PMALL=PTALL=PQALL=POALL=NULL;

	for (auto it = tRoot.begin(); it != tRoot.end(); ++it) {

		auto each = it->second;

		user = each.get<string>("user");
		mqid = each.get<string>("mqid");
		t1 = each.get_child("subscribed");
		t2 = each.get_child("subcodes");

                iStockCode=-1;

		for (auto it2 = t2.begin(); it2 != t2.end(); ++it2) {


			iStockCode=it2->second.get_value<int>();

			/*过滤无效的代码*/
			if(iStockCode<=0||iStockCode>MAX_STOCK_CODE) continue;

			for (auto it1 = t1.begin(); it1 != t1.end(); ++it1) {

				iSubscribed=it1->second.get_value<int>();

				switch(iSubscribed){
				case 12:AUSER=&AMUSER[0];break;
				case 13:AUSER=&ATUSER[0];break;
				case 14:AUSER=&AQUSER[0];break;
				case 15:AUSER=&AOUSER[0];break;
				default:AUSER=NULL;
				break;
				}
				if(AUSER==NULL)break;

				pTemp=new (struct UserStruct);

				if(pTemp==NULL){
					printf("error new struct UserStruct.\n");
					exit(1);
				}

				pTemp->pNext=NULL;
				strncpy(pTemp->sUserName,user.c_str(),sizeof(pTemp->sUserName)-1);
				pTemp->sUserName[sizeof(pTemp->sUserName)-1]=0;

				pTemp->iMqId=	atoi(mqid.c_str());


				/*插入到表中*/
				pTemp->pNext=AUSER[iStockCode];
				AUSER[iStockCode]=pTemp;
                	}
                }

                if(iStockCode==-1){

			for (auto it1 = t1.begin(); it1 != t1.end(); ++it1) {

				iSubscribed=it1->second.get_value<int>();

	printf("hello world.--------------------------------------------5.yyyy.\n");

				switch(iSubscribed){
				case 12:PPALL=&PMALL;break;
				case 13:PPALL=&PTALL;break;
				case 14:PPALL=&PQALL;break;
				case 15:PPALL=&POALL;break;
				default:PPALL=NULL;
				}
				if(PPALL==NULL)break;

				pTemp=new (struct UserStruct);

				if(pTemp==NULL){
					printf("error new struct UserStruct.\n");
					exit(1);
				}

				pTemp->pNext=NULL;
				strncpy(pTemp->sUserName,user.c_str(),sizeof(pTemp->sUserName)-1);
				pTemp->sUserName[sizeof(pTemp->sUserName)-1]=0;

				pTemp->iMqId=	atoi(mqid.c_str());

				/*插入到表中*/
				pTemp->pNext=*PPALL;
				*PPALL=pTemp;
			}
                }

	}
	printf("hello world.--------------------------------------------5.8.\n");

}
int SendMsg2Mq(string &str,int iMqId)
{
	MessageQueue *mq=new MessageQueue(iMqId);
	
	if(mq==NULL) return -1;
	
//	open(bool ifblock, bool ifcreate, int maxlen, int maxnum);

	mq->open(false,false,4096,1);
	mq->send(str,0);
	
	delete mq;

	return 0;
}
void SendMsg2Cli(int iStockCode,char cType,string& str)
{
	char sBuffer[4];
	unsigned int len,l0,l1;
	struct UserStruct *pAll,*pUser;
	
	string str1;
	
//	printf("hello world.--------------------------------------------3.5.\n");

	if(IsWorkThreadLock()){
		
		printf("hello world.--------------------------------------------4.\n");
		FlashUserArray(sFlashDispName);
		UnLockWorkThread();
	}

	/*取字节长度，并判断最大数值*/
	len=str.length()+3; if(len>10230) len=10230;

	l0=len%256;
	l1=len/256;

	((unsigned char*)sBuffer)[0]=l1;
	((unsigned char*)sBuffer)[1]=l0;

	switch (cType){
	case 'M': pUser=AMUSER[iStockCode];pAll=PMALL;sBuffer[2]=12;break;
	case 'T': pUser=ATUSER[iStockCode];pAll=PTALL;sBuffer[2]=13;break;
	case 'Q': pUser=AQUSER[iStockCode];pAll=PQALL;sBuffer[2]=14;break;
	case 'O': pUser=AOUSER[iStockCode];pAll=POALL;sBuffer[2]=15;break;
	default:  pUser=AQUSER[iStockCode];pAll=PQALL;sBuffer[2]=14;break;
	break;
	}

	str1=string(sBuffer,3)+str;
	
//	strncpy(sBuffer+3,str.c_str(),str.length());

	while(pUser!=NULL){
		SendMsg2Mq(str1,pUser->iMqId);
		pUser=pUser->pNext;
	}

	while(pAll!=NULL){
		SendMsg2Mq(str1,pAll->iMqId);
		pAll=pAll->pNext;
	}
}
CallBackBase::CallBackBase(int iWriteFlag,string& strWork)
{
	m_ios = 0;

	if(iWriteFlag==1){
		m_fileSet.gtaqhName=	strWork+"/gta_qh.dat";
		m_fileSet.gtaahName=	strWork+"/gta_ah.dat";
		m_fileSet.gtathName=	strWork+"/gta_th.dat";
		m_fileSet.gtaqzName=	strWork+"/gta_qz.dat";
		m_fileSet.gtatzName=	strWork+"/gta_tz.dat";
		m_fileSet.gtaozName=	strWork+"/gta_oz.dat";
		
		m_fileSet.fpGtaQh=	fopen(m_fileSet.gtaqhName.c_str(),"ab+");
		m_fileSet.fpGtaAh=	fopen(m_fileSet.gtaahName.c_str(),"ab+");
		m_fileSet.fpGtaTh=	fopen(m_fileSet.gtathName.c_str(),"ab+");
		m_fileSet.fpGtaQz=	fopen(m_fileSet.gtaqzName.c_str(),"ab+");
		m_fileSet.fpGtaTz=	fopen(m_fileSet.gtatzName.c_str(),"ab+");
		m_fileSet.fpGtaOz=	fopen(m_fileSet.gtaozName.c_str(),"ab+");

	}
	else if(iWriteFlag==2){
		m_fileSet.tdfmktName=	strWork+"/tdf_mkt.dat";
		m_fileSet.tdfqueName=	strWork+"/tdf_que.dat";
		m_fileSet.tdftraName=	strWork+"/tdf_tra.dat";
		m_fileSet.tdfordName=	strWork+"/tdf_ord.dat"; 

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

	if(m_iWriteFlag==1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaQh);
		fwrite((const void*)RealSSEL2Quotation,sizeof(SSEL2_Quotation),1,m_fileSet.fpGtaQh);
		fflush(m_fileSet.fpGtaQh);
	}
	else if(m_iWriteFlag==2){

		fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,m_fileSet.fpTdfMkt);
		fwrite((const void*)&q[0],sizeof(TDF_ORDER_QUEUE),2,m_fileSet.fpTdfQue);

		fflush(m_fileSet.fpTdfMkt);
		fflush(m_fileSet.fpTdfQue);
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
	string strTr,strOq0,strOq1;

	for(int i=0;i<max_trans_cnt;i++)
	GTA2TDF_SSEL2_T(RealSSEL2Transaction[0],t);

	if(m_iWriteFlag==1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaTh);
		fwrite((const void*)RealSSEL2Transaction,sizeof(SSEL2_Transaction),1,m_fileSet.fpGtaTh);
		fflush(m_fileSet.fpGtaTh);
	}
	else if(m_iWriteFlag==2){
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

	if(m_iWriteFlag==1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaAh);
		fwrite((const void*)RealSSEL2Auction,sizeof(SSEL2_Auction),1,m_fileSet.fpGtaAh);
		fflush(m_fileSet.fpGtaAh);
	}
	else if(m_iWriteFlag==2){
		fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,m_fileSet.fpTdfMkt);
		fflush(m_fileSet.fpTdfMkt);
	}

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

	if(m_iWriteFlag==1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaQz);
		fwrite((const void*)RealSZSEL2Quotation,sizeof(SZSEL2_Quotation),1,m_fileSet.fpGtaQz);
		fflush(m_fileSet.fpGtaQz);
	}
	else if(m_iWriteFlag==2){

		fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,m_fileSet.fpTdfMkt);
		fwrite((const void*)&q[0],sizeof(TDF_ORDER_QUEUE),2,m_fileSet.fpTdfQue);
		fflush(m_fileSet.fpTdfMkt);
		fflush(m_fileSet.fpTdfQue);
	}

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
	string strTr,strOq0,strOq1;

	for(int i=0;i<max_trans_cnt;i++)
	GTA2TDF_SZSEL2_T(RealSZSEL2Transaction[0],t);

	if(m_iWriteFlag==1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaTz);
		fwrite((const void*)RealSZSEL2Transaction,sizeof(SZSEL2_Transaction),1,m_fileSet.fpGtaTz);
		fflush(m_fileSet.fpGtaTz);
	}
	else if(m_iWriteFlag==2){
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

	if(m_iWriteFlag==1){
		fwrite((const void*)&(subdata->cur_time),sizeof(subdata->cur_time),1,m_fileSet.fpGtaOz);
		fwrite((const void*)RealSZSEL2Order,sizeof(SZSEL2_Order),1,m_fileSet.fpGtaOz);
		fflush(m_fileSet.fpGtaOz);
	}
	else if(m_iWriteFlag==2){
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

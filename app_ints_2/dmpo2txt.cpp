
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include "wwtiny.h"
#include "QTSDataType.h"
#include "QTSStruct.h"
#include "QTSDataFieldDefine.h"

#include "TDFAPIStruct.h"

#include <iostream>
#include <string>
using namespace std;

#include "mktdata.pb.h"

#define MY_TYPE_TDF_MKT			'M'
#define MY_TYPE_TDF_TRA			'T'
#define MY_TYPE_TDF_ORD			'O'
#define MY_TYPE_TDF_QUE			'Q'


#define MY_DATE_CEIL_LONG 1000000000L

int64_t my_yuan2percentFen(const double yuan)
{
	return int64_t((yuan + 0.00005) * 10000);
}
int print_MY_TYPE_TDF_MKT(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	struct TDF_MARKET_DATA   *p=(struct TDF_MARKET_DATA *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->szCode)==NULL) return 1;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\n",
			"picktime",
			"time",
			"stockcode",
			"difftime");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->nTime;break;
		case 2: *plCurTime=lTmpTime;break;
		default:*plCurTime=p->nTime;break;
	}
	if(*plCurTime<lBgnTime) return 2;


	sprintf(outbuf,"%lld\t%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\n",
		*(long long *)buf,	//picktime
		(int)p->nTime,		//委托时间YYYYMMDDHHMMSSMMM
		p->szCode,
		iSubMilliSec((int)lTmpTime,(int)p->nTime),//第一行继续
		(int)p->nStatus,	//状态
		(int)p->nPreClose,	//前收盘
		(int)p->nOpen,		//开盘价
		(int)p->nHigh,		//最高价
		(int)p->nLow,		//最低价
		(int)p->nMatch,		//最新价	//第二行开始
		(int)p->nAskPrice[0],
		(int)p->nAskPrice[1],
		(int)p->nAskPrice[2],
		(int)p->nAskPrice[3],
		(int)p->nAskPrice[4],
		(int)p->nAskPrice[5],
		(int)p->nAskPrice[6],
		(int)p->nAskPrice[7],
		(int)p->nAskPrice[8],
		(int)p->nAskPrice[9],
		(int)p->nAskVol[0],
		(int)p->nAskVol[1],
		(int)p->nAskVol[2],
		(int)p->nAskVol[3],
		(int)p->nAskVol[4],
		(int)p->nAskVol[5],
		(int)p->nAskVol[6],
		(int)p->nAskVol[7],
		(int)p->nAskVol[8],
		(int)p->nAskVol[9],	//第三行开始
		(int)p->nBidPrice[0],
		(int)p->nBidPrice[1],
		(int)p->nBidPrice[2],
		(int)p->nBidPrice[3],
		(int)p->nBidPrice[4],
		(int)p->nBidPrice[5],
		(int)p->nBidPrice[6],
		(int)p->nBidPrice[7],
		(int)p->nBidPrice[8],
		(int)p->nBidPrice[9],
		(int)p->nBidVol[0],
		(int)p->nBidVol[1],
		(int)p->nBidVol[2],
		(int)p->nBidVol[3],
		(int)p->nBidVol[4],
		(int)p->nBidVol[5],
		(int)p->nBidVol[6],
		(int)p->nBidVol[7],
		(int)p->nBidVol[8],
		(int)p->nBidVol[9],	//第四行开始
		(int)p->nNumTrades,			//成交笔数
		(int)p->iVolume,			//成交总量
		(int)p->iTurnover,			//成交总金额
		(int)p->nTotalBidVol,			//委托买入总量
		(int)p->nTotalAskVol,			//委托卖出总量
		(int)p->nWeightedAvgBidPrice,		//加权平均委买价格
		(int)p->nWeightedAvgAskPrice,		//加权平均委卖价格
		(int)p->nIOPV,				//IOPV净值估值
		(int)p->nYieldToMaturity,		//到期收益率
		(int)p->nHighLimited,			//涨停价
		(int)p->nLowLimited,			//跌停价
		p->chPrefix,				//证券信息前缀
		(int)p->nSyl1,				//市盈率1
		(int)p->nSyl2,				//市盈率2
		(int)p->nSD2				//升跌2（对比上一笔）
		);

	return 0;
}

int print_MY_TYPE_TDF_TRA(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	struct TDF_TRANSACTION   *p=(TDF_TRANSACTION *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->szCode)==NULL) return 1;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\n",
			"picktime",
			"tradetime",
			"stockcode",
			"difftime");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->nTime;break;
		case 2: *plCurTime=lTmpTime;break;
		default:*plCurTime=p->nTime;break;
	}
	if(*plCurTime<lBgnTime) return 2;

	if(p->chOrderKind==0) 	p->chOrderKind='0';
	else			p->chOrderKind='1';
	if(p->chFunctionCode==0) p->chFunctionCode='0';

	sprintf(outbuf,"%lld\t%d\t%s\t%d\
\t%d\t%d\t%d\t%d\t%c\t%c\t%c\t%d\t%d\n",
		*(long long *)buf,	//picktime
		(int)p->nTime,//委托时间YYYYMMDDHHMMSSMMM
		p->szCode,
		iSubMilliSec((int)lTmpTime,(int)p->nTime),//第二行开始
		(int)p->nIndex,            //成交编号
		(int)p->nPrice,            //成交价格
		(int)p->nVolume,	        //成交数量
		(int)p->nTurnover,	            //成交金额
		(char)p->nBSFlag,                    //买卖方向(买：'B', 卖：'S', 不明：' ')
		p->chOrderKind,                //成交类别
		p->chFunctionCode,             //成交代码
		(int)p->nAskOrder,	                //叫卖方委托序号
		(int)p->nBidOrder	                //叫买方委托序号
		);

	return 0;
}

int print_MY_TYPE_TDF_ORD(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	struct TDF_ORDER   *p=(TDF_ORDER *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->szCode)==NULL) return 1;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\n",
			"picktime",
			"ordertime",
			"stockcode",
			"difftime");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->nTime;break;
		case 2: *plCurTime=lTmpTime;break;
		default:*plCurTime=p->nTime;break;
	}
	if(*plCurTime<lBgnTime) return 2;

	if(p->chOrderKind==0) p->chOrderKind='0';
	if(p->chFunctionCode==0) p->chFunctionCode='0';
	if(p->chStatus==0) 	p->chStatus='0';
	if(p->chFlag==0)	p->chFlag='0';

	sprintf(outbuf,"%lld\t%d\t%s\t%d\
\t%d\t%d\t%d\t%c\t%c\t%d\t%c\t%c\n",
		*(long long *)buf,	//picktime
		(int)p->nTime,//委托时间YYYYMMDDHHMMSSMMM
		p->szCode,
		iSubMilliSec((int)lTmpTime,(int)p->nTime),//第二行开始
		(int)p->nOrder,	        	//委托号
		(int)p->nPrice,         	//委托价格
		(int)p->nVolume,		//委托数量
		p->chOrderKind,                //委托类别
		p->chFunctionCode,             //委托代码('B','S','C')
		(int)p->nBroker,		//经纪商编码(PT市场有效)
		p->chStatus,			//委托状态(PT市场有效)
		p->chFlag			//标志(PT市场有效)
		);

	return 0;
}

int print_MY_TYPE_TDF_QUE(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	struct TDF_ORDER_QUEUE   *p=(TDF_ORDER_QUEUE *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->szCode)==NULL) return 1;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\n",
			"picktime",
			"time",
			"stockcode",
			"difftime");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->nTime;break;
		case 2: *plCurTime=lTmpTime;break;
		default:*plCurTime=p->nTime;break;
	}
	if(*plCurTime<lBgnTime) return 2;

	sprintf(outbuf,"%lld\t%d\t%s\t%d\
\t%c\t%d\t%d\t%d\t%d\n",
		*(long long *)buf,	//picktime
		(int)p->nTime,//委托时间YYYYMMDDHHMMSSMMM
		p->szCode,
		iSubMilliSec((int)lTmpTime,(int)p->nTime),//第二行开始
		(char)p->nSide,			//买卖方向('B':Bid 'A':Ask)
		(int)p->nPrice,         	//委托价格
		(int)p->nOrders,		//订单数量
		(int)p->nABItems,		//明细个数
		(int)p->nABVolume[0]		//订单明细
		);

	return 0;
}

void MktData2TDF_MARKET_DATA(MktData &md, TDF_MARKET_DATA *po)
{
	bzero((void*)po,sizeof(TDF_MARKET_DATA));

	sprintf(po->szCode,"%06d",md.szcode());
	po->nTime=	md.ntime();
	po->nStatus=	md.nstatus();
	po->nPreClose=	md.npreclose();
	po->nOpen=	md.nopen();
	po->nHigh=	md.nhigh();
	po->nLow=	md.nlow();
	po->nMatch=	md.nmatch();

	for(int i=0;i<10;i++){
		po->nAskPrice[i]=	md.naskprice(i);
		po->nAskVol[i]=		md.naskvol(i);;
		po->nBidPrice[i]=	md.nbidprice(i);
		po->nBidVol[i]=		md.nbidvol(i);
	}

	po->nNumTrades=			md.nnumtrades();
	po->iVolume=                	md.ivolume();
	po->iTurnover=                  md.iturnover();
	po->nTotalBidVol=               md.ntotalbidvol();
	po->nTotalAskVol=               md.ntotalaskvol();
	po->nWeightedAvgBidPrice=       md.nweightedavgbidprice();
	po->nWeightedAvgAskPrice=       md.nweightedavgaskprice();
	po->nIOPV=                      md.niopv();
	po->nYieldToMaturity=           md.nyieldtomaturity();
	po->nHighLimited=               md.nhighlimited();
	po->nLowLimited=                md.nlowlimited();
	po->nSyl1=                      md.nsyl1();
	po->nSyl2=                      md.nsyl2();
	po->nSD2=                       md.nsd2();
}

int convert_MY_TYPE_TDF_MKT(char *pi,long lRecLen,char *po)
{
	string msg;

	MktData         md;

	msg.assign(pi, lRecLen);

	md.ParseFromString(msg);

	MktData2TDF_MARKET_DATA(md,(TDF_MARKET_DATA*)po);

	return 0;
}

void Transaction2TDF_TRANSACTION(Transaction &tr, TDF_TRANSACTION *po)
{
	bzero((void*)po,sizeof(TDF_TRANSACTION));

	sprintf(po->szCode,"%06d",tr.szcode());

	po->nTime=		tr.ntime();
	po->nIndex=		tr.nindex();
	po->nPrice=		tr.nprice();
	po->nVolume=		tr.nvolume();
	po->nTurnover=		tr.nturnover();
	po->nBSFlag=		tr.nbsflag();
	po->chOrderKind=	tr.chorderkind();
	po->chFunctionCode=	tr.chfunctioncode();
	po->nAskOrder=		tr.naskorder();
	po->nBidOrder=		tr.nbidorder();
}

int convert_MY_TYPE_TDF_TRA(char *pi,long lRecLen,char *po)
{
	string msg;

	Transaction         tr;

	msg.assign(pi, lRecLen);

	tr.ParseFromString(msg);

	Transaction2TDF_TRANSACTION(tr,(TDF_TRANSACTION*)po);

	return 0;
}
void Order2TDF_ORDER(Order &od, TDF_ORDER *po)
{
	
	bzero((void*)po,sizeof(TDF_ORDER));

	sprintf(po->szCode,"%06d",od.szcode());

	po->nTime=		od.ntime();

	po->nOrder=		od.norder();
	po->nPrice=		od.nprice();
	po->nVolume=		od.nvolume();


	po->chOrderKind=	od.chorderkind();
	po->chFunctionCode=	od.chfunctioncode();
	po->nBroker=		od.nbroker();
	po->chStatus=		od.chstatus();  
	po->chFlag=		od.chflag();
}

int convert_MY_TYPE_TDF_ORD(char *pi,long lRecLen,char *po)
{
	string msg;

	Order         od;

	msg.assign(pi, lRecLen);

	od.ParseFromString(msg);

	Order2TDF_ORDER(od,(TDF_ORDER*)po);

	return 0;
}
void Order_queue2TDF_ORDER_QUEUE(Order_queue &oq, TDF_ORDER_QUEUE *po)
{
	bzero((void*)po,sizeof(TDF_ORDER_QUEUE));

	sprintf(po->szCode,"%06d",oq.szcode());

	po->nTime=		oq.ntime();
	po->nSide=		oq.nside()==1?'B':'A';
//	po->nSide=		(po->nSide==1?'B':'A');
	
	po->nPrice=		oq.nprice();
	po->nOrders=		oq.norders();
	po->nABItems=		oq.nabitems();
	
	for (int i = 0; i < po->nABItems; i++)
		po->nABVolume[i]=oq.nabvolume(i);
}
int convert_MY_TYPE_TDF_QUE(char *pi,long lRecLen,char *po)
{
	string msg;

	Order_queue         oq;

	msg.assign(pi, lRecLen);

	oq.ParseFromString(msg);

	Order_queue2TDF_ORDER_QUEUE(oq,(TDF_ORDER_QUEUE*)po);

	return 0;
}
int (*convert_MY_TYPE)(char *pi,long lRecLen,char *po);
int (*print_MY_TYPE)(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf);

#define MY_GET_REC_LEN(p)  (((unsigned char*)p)[8]*256L+((unsigned char*)p)[9])

char MyGetRecType(char *p)
{
	static char *TypeMapStr=(char*)"MTQO";

	if(p[10]>=12&&p[10]<=15)
		return TypeMapStr[p[10]-12];
	return 'M';
}

long lGetSkipPos(char sFileName[],int iSkipCnt)
{
	long lCurPos=0;
	FILE *fp;
	char sBuf[16];
	unsigned char *p=(unsigned char *)(sBuf+8);

	if(iSkipCnt<=0) return 0L;

	if((fp=fopen(sFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sFileName);
		return -1L;
	}

	while(iSkipCnt>0){
		if(fread((void*)sBuf,10,1,fp)!=1)
			break;

		lCurPos+=(long)(p[0]*256L+p[1]);

		if(fseek(fp,lCurPos,SEEK_SET)<0){
			perror("seek file");
			fclose(fp);
			return -1;
		}
		iSkipCnt--;
	}

	fclose(fp);

	return lCurPos;
}
int main(int argc, char *argv[])
{
	int iType=1,ret,i=0,iSkipCnt=0,iFilterCnt=0,iMaxCount=100;
	int iExceedCnt=0,iDelayCnt=0,iDelaySec=10,iTimeFlag=1;
	long lBgnTime=0,lEndTime=999999999999,lCurTime=0,lItemLen=0,lCurPos,lRecLen;

	char cType,sInFileName[1024],sCodeStr[1024],sBuffer[1024],sProtoBuf[1024],sOutBuf[10240];

	FILE *fpIn;

	for (int c; (c = getopt(argc, argv, "i:s:c:b:e:l:t:d:f:")) != EOF;){

		switch (c){
		case 'i':strcpy(sInFileName, optarg);	break;
		case 's':iSkipCnt=atoi(optarg);		break;
		case 'c':iMaxCount=atoi(optarg);	break;
		case 'b':lBgnTime=atol(optarg);		break;
		case 'e':lEndTime=atol(optarg);		break;
		case 'l':strcpy(sCodeStr, optarg);	break;
		case 't':
			//增加TDF的四种类型
			switch (optarg[0]){
				case 'm':case 'M':iType='M';break;
				case 't':case 'T':iType='T';break;
				case 'o':case 'O':iType='O';break;
				case 'q':case 'Q':iType='Q';break;
				default:iType='M';break;
			}
			break;
		case 'd':iDelaySec=atoi(optarg);	break;
		case 'f':iTimeFlag=atoi(optarg);	break;
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-i infile-name ]\n");
			printf("   [-s skipcnt (def=0)]\n");
			printf("   [-c count (def=100,-1=all) ]\n");
			printf("   [-b begin-time (def=0) ]\n");
			printf("   [-e end-time (def=99999999999) ]\n");
			printf("   [-t type (mtoq|MTOQ=>mkt,trs,ord,que) ]\n");
			printf("   [-d delay sec (-e,-l multi-code effect) ]\n");
			printf("   [-l codelist (e.g \"000001,603912,002415\") ]\n");
			printf("   [-f time_flag (def=1,1-createtime,2-picktime) ]\n");
			exit(1);
			break;
		}
	}

	switch (iType){
		case MY_TYPE_TDF_MKT:
		lItemLen=(sizeof(TDF_MARKET_DATA));
		print_MY_TYPE=print_MY_TYPE_TDF_MKT;
		convert_MY_TYPE=convert_MY_TYPE_TDF_MKT;
		break;
		case MY_TYPE_TDF_TRA:
		lItemLen=(sizeof(TDF_TRANSACTION));
		print_MY_TYPE=print_MY_TYPE_TDF_TRA;
		convert_MY_TYPE=convert_MY_TYPE_TDF_TRA;
		break;
		case MY_TYPE_TDF_ORD:
		lItemLen=(sizeof(TDF_ORDER));
		print_MY_TYPE=print_MY_TYPE_TDF_ORD;
		convert_MY_TYPE=convert_MY_TYPE_TDF_ORD;
		break;
		case MY_TYPE_TDF_QUE:
		lItemLen=(sizeof(TDF_ORDER_QUEUE));
		print_MY_TYPE=print_MY_TYPE_TDF_QUE;
		convert_MY_TYPE=convert_MY_TYPE_TDF_QUE;
		break;
		default:
			lItemLen=(sizeof(TDF_MARKET_DATA));
			print_MY_TYPE=print_MY_TYPE_TDF_MKT;
			convert_MY_TYPE=convert_MY_TYPE_TDF_MKT;
		break;
	}
	lItemLen+=sizeof(long long);

	if((lCurPos=lGetSkipPos(sInFileName,iSkipCnt))<0) return -1;


	if((fpIn=fopen(sInFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sInFileName);
		return -1;
	}

	if(fseek(fpIn,lCurPos,SEEK_SET)<0){
		perror("seek file");
		return -1;
	}

	while(1){
		//size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
		if(fread((void*)sBuffer,11,1,fpIn)!=1){
			printf("end of file break.\n");
			break;
		}

		//拿到记录长度
		lRecLen=MY_GET_REC_LEN(sBuffer)-1;
		cType=MyGetRecType(sBuffer);

		if(cType!=iType){
			if(fseek(fpIn,lRecLen,SEEK_CUR)<0){
				perror("seek file");
				return -1;
			}
			iSkipCnt++;
			continue;
		}

		if(fread((void*)sProtoBuf,lRecLen,1,fpIn)!=1){
			printf("end of file break.\n");
			break;
		}
		//将protobuf格式解码为二进制结构
		if(convert_MY_TYPE(sProtoBuf,lRecLen,sBuffer+8)<0){
			perror("seek file");
			return -1;

		}
		//将整理好的格式，转换为文本
		if((ret=print_MY_TYPE(sBuffer,sCodeStr,iTimeFlag,lBgnTime,&lCurTime,sOutBuf))!=0){
			if(ret==1)	iFilterCnt++;
			else		iSkipCnt++;
			continue;
		}
		//如果发现所选的股票的时间大于命令参数指定时间
		if(lCurTime>lEndTime){

			//如果只选一只股票，则直接break
			if(strlen(sCodeStr)==6) break;

			//如果选多支股票，或全选，则容许跳过10秒,
			//为了避免因为一只股票超时，而导致其他股票的endtime之前的数据没取了
			if(lCurTime>(lEndTime+iDelaySec*1000L)) break;

			iExceedCnt++;
			continue;
		}
		printf("%s",sOutBuf);

		//如果发现股票的时间到达了，统计一下超过这个时间之后的还有多少条选中信息
		if(iExceedCnt>0) iDelayCnt++;

		if((++i)>=iMaxCount) break;

	}

	printf("total skip cnt = %d filter cnt =%d exceed cnt =%d delay cnt =%d output cnt =%d.\n",
		iSkipCnt,iFilterCnt,iExceedCnt,iDelayCnt,i);

	fclose(fpIn);

	return 0;
}

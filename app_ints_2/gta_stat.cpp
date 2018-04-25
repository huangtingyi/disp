
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>


#include "QTSDataType.h"
#include "QTSStruct.h"
#include "QTSDataFieldDefine.h"

#include "../GTA2TDF/GTA2TDF.h"

#define MY_TYPE_SSEL2_Quotation		1
#define MY_TYPE_SSEL2_Transaction	2
#define MY_TYPE_SSEL2_Auction		3
#define MY_TYPE_SZSEL2_Quotation	4
#define MY_TYPE_SZSEL2_Transaction	5
#define MY_TYPE_SZSEL2_Order		6

#define MY_DATE_CEIL_LONG 1000000000L

int print_MY_TYPE_SSEL2_Quotation(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	SSEL2_Quotation   *p=(SSEL2_Quotation *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"createtime",
			"stockcode",
			"difftime");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);
	
	switch(iTimeFlag){
		case 1: *plCurTime=p->Time;break;
		case 2: *plCurTime=lTmpTime;break;
		case 3:	*plCurTime=p->LocalTimeStamp;break;
		case 4:	*plCurTime=(long)(p->PacketTimeStamp%MY_DATE_CEIL_LONG);break;
		default:*plCurTime=p->Time;break;
	}
	
	if(*plCurTime<lBgnTime) return 2;

	sprintf(outbuf,"%lld\t%d\t%lld\t%d\t%s\t%ld\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->Time,		//< 数据生成时间, 最新订单时间（毫秒）;143025001 表示 14:30:25.001
		p->Symbol,
		lTmpTime-p->Time);

	return 0;
}

int print_MY_TYPE_SSEL2_Transaction(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	SSEL2_Transaction   *p=(SSEL2_Transaction *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"tradetime",
			"stockcode",
			"difftime");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->TradeTime;break;
		case 2: *plCurTime=lTmpTime;break;
		case 3:	*plCurTime=p->LocalTimeStamp;break;
		case 4:	*plCurTime=(long)(p->PacketTimeStamp%MY_DATE_CEIL_LONG); break;
		default:*plCurTime=p->TradeTime;break;
	}
	
	if(*plCurTime<lBgnTime) return 2;

	sprintf(outbuf,"%lld\t%d\t%lld\t%d\t%s\t%ld\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->TradeTime,		//< 成交时间(毫秒), 14302506 表示14:30:25.06
		p->Symbol,
		lTmpTime-p->TradeTime);

	return 0;
}
int64_t my_yuan2percentFen(const double yuan) 
{
	return int64_t((yuan + 0.00005) * 10000);
}
int print_MY_TYPE_SSEL2_Auction(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	SSEL2_Auction   *p=(SSEL2_Auction *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

//	TDF_MARKET_DATA m;

//	GTA2TDF_SSEL2_AM(p[0],m);

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"createtime",
			"stockcode",
			"difftime",
			"quotation_flag",
			"open_price",
			"auction_volume",
			"leave_volume",
			"side");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->Time;break;
		case 2: *plCurTime=lTmpTime;break;
		case 3:	*plCurTime=p->LocalTimeStamp;break;
		case 4:	*plCurTime=(long)(p->PacketTimeStamp%MY_DATE_CEIL_LONG);break;
		default:*plCurTime=p->Time;break;
	}
	
	if(*plCurTime<lBgnTime) return 2;

	sprintf(outbuf,"%lld\t%d\t%lld\t%d\t%s\t%ld\t%s\t%d\t%d\t%d\t%c\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->Time,		//< 数据生成时间, 143025001 表示 14:30:25.001
		p->Symbol,
		lTmpTime-p->Time,
		p->QuotationFlag,
		(int)my_yuan2percentFen(p->OpenPrice),
		(int)p->AuctionVolume,
		(int)p->LeaveVolume,
		p->Side);

	return 0;
}
int print_MY_TYPE_SZSEL2_Quotation(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	SZSEL2_Quotation   *p=(SZSEL2_Quotation *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

	*plCurTime=p->Time%MY_DATE_CEIL_LONG;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"createtime",
			"stockcode",
			"difftime");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->Time%MY_DATE_CEIL_LONG;break;
		case 2: *plCurTime=lTmpTime;break;
		case 3:	*plCurTime=p->LocalTimeStamp;break;
		default:*plCurTime=p->Time;break;
	}
	if(*plCurTime<lBgnTime) return 2;

	sprintf(outbuf,"%lld\t%d\t%ld\t%d\t%s\t%ld\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 无数值
		(int)(p->Time%MY_DATE_CEIL_LONG),//< 数据生成时间YYYYMMDDHHMMSSMMM
		p->Symbol,
		lTmpTime-(long)(p->Time%MY_DATE_CEIL_LONG));

	return 0;
}
int print_MY_TYPE_SZSEL2_Transaction(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	SZSEL2_Transaction   *p=(SZSEL2_Transaction *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"tradetime",
			"stockcode",
			"difftime"
			);
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->TradeTime%MY_DATE_CEIL_LONG;break;
		case 2: *plCurTime=lTmpTime;break;
		case 3:	*plCurTime=p->LocalTimeStamp;break;
		default:*plCurTime=p->TradeTime;break;
	}
	if(*plCurTime<lBgnTime) return 2;

	sprintf(outbuf,"%lld\t%d\t%ld\t%d\t%s\t%ld\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		(int)(p->TradeTime%MY_DATE_CEIL_LONG),//成交时间YYYYMMDDHHMMSSMMM
		p->Symbol,
		lTmpTime-(long)(p->TradeTime%MY_DATE_CEIL_LONG)
		);

	return 0;
}
int print_MY_TYPE_SZSEL2_Order(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf)
{
	SZSEL2_Order   *p=(SZSEL2_Order *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"ordertime",
			"stockcode",
			"difftime");
	}

	long lTmpTime=(long)((*(long long *)buf)%MY_DATE_CEIL_LONG);

	switch(iTimeFlag){
		case 1: *plCurTime=p->Time%MY_DATE_CEIL_LONG;break;
		case 2: *plCurTime=lTmpTime;break;
		case 3:	*plCurTime=p->LocalTimeStamp;break;
		default:*plCurTime=p->Time;break;
	}
	if(*plCurTime<lBgnTime) return 2;


	sprintf(outbuf,"%lld\t%d\t%ld\t%d\t%s\t%ld\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		(int)(p->Time%MY_DATE_CEIL_LONG),//委托时间YYYYMMDDHHMMSSMMM
		p->Symbol,
		lTmpTime-(long)(p->Time%MY_DATE_CEIL_LONG));

	return 0;
}


int (*print_MY_TYPE)(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf);

int main(int argc, char *argv[])
{
	int iType=1,ret,i=0,iSkipCnt=0,iFilterCnt=0,iMaxCount=100;
	int iExceedCnt=0,iDelayCnt=0,iDelaySec=10,iTimeFlag=1;
	long lBgnTime=0,lEndTime=999999999999,lCurTime=0,lItemLen=0;

	char sInFileName[1024],sCodeStr[1024],sBuffer[10240],sOutBuf[10240];

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
			iType=atoi(optarg);
			if(iType<=0||iType>6) iType=1;
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
			printf("   [-t type (1-qh,2-th,3-ah,4-qz,5-tz,6-oz) ]\n");
			printf("   [-d delay sec (-e,-l multi-code effect) ]\n");        
			printf("   [-f time_flag (def=1,1-createtime,2-localtime,3 picktime) ]\n");
			exit(1);
			break;
		}
	}


	if((fpIn=fopen(sInFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sInFileName);
		return -1;
	}

	switch (iType){
		case MY_TYPE_SSEL2_Quotation	:
		lItemLen=(sizeof(SSEL2_Quotation));
		print_MY_TYPE=print_MY_TYPE_SSEL2_Quotation;
		break;
		case MY_TYPE_SSEL2_Transaction:
		lItemLen=(sizeof(SSEL2_Transaction));
		print_MY_TYPE=print_MY_TYPE_SSEL2_Transaction;
		break;
		case MY_TYPE_SSEL2_Auction:
		lItemLen=(sizeof(SSEL2_Auction));
		print_MY_TYPE=print_MY_TYPE_SSEL2_Auction;
		break;
		case MY_TYPE_SZSEL2_Quotation:
		lItemLen=(sizeof(SZSEL2_Quotation));
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Quotation;
		break;
		case MY_TYPE_SZSEL2_Transaction:
		lItemLen=(sizeof(SZSEL2_Transaction));
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Transaction;
		break;
		case MY_TYPE_SZSEL2_Order:
		lItemLen=(sizeof(SZSEL2_Order));
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Order;
		break;
		default:
		lItemLen=(sizeof(SSEL2_Quotation));
		print_MY_TYPE=print_MY_TYPE_SSEL2_Quotation;
		break;
	}
	lItemLen+=sizeof(long long);

	if(fseek(fpIn,lItemLen*iSkipCnt,SEEK_SET)<0){
		perror("seek file");
		return -1;
	}

	while(1){
		//size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
		if(fread((void*)sBuffer,lItemLen,1,fpIn)!=1){
			printf("end of file break.\n");
			break;
		}

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

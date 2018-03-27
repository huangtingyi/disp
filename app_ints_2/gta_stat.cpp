
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>


#include "QTSDataType.h"
#include "QTSStruct.h"
#include "QTSDataFieldDefine.h"

#define MY_TYPE_SSEL2_Quotation		1
#define MY_TYPE_SSEL2_Transaction	2
#define MY_TYPE_SSEL2_Auction		3
#define MY_TYPE_SZSEL2_Quotation	4
#define MY_TYPE_SZSEL2_Transaction	5
#define MY_TYPE_SZSEL2_Order		6



int comp_time_MY_TYPE_SSEL2_Quotation(char *buf,long lBgnTime)
{return ((SSEL2_Quotation*)buf)->Time<lBgnTime;}

int comp_time_MY_TYPE_SSEL2_Transaction(char *buf,long lBgnTime)
{return ((SSEL2_Transaction*)buf)->TradeTime<lBgnTime;}

int comp_time_MY_TYPE_SSEL2_Auction(char *buf,long lBgnTime)
{return ((SSEL2_Auction*)buf)->Time<lBgnTime;}

int comp_time_MY_TYPE_SZSEL2_Quotation(char *buf,long lBgnTime)
{return ((SZSEL2_Quotation*)buf)->Time%1000000000<lBgnTime;}

int comp_time_MY_TYPE_SZSEL2_Transaction(char *buf,long lBgnTime)
{return ((SZSEL2_Transaction*)buf)->TradeTime%1000000000<lBgnTime;}

int comp_time_MY_TYPE_SZSEL2_Order(char *buf,long lBgnTime)
{return ((SZSEL2_Order*)buf)->Time%1000000000<lBgnTime;}


int print_MY_TYPE_SSEL2_Quotation(char *buf,char sCodeList[],long *plCurTime)
{
	SSEL2_Quotation   *p=(SSEL2_Quotation *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;
	
	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"createtime",
			"stockcode");
	}
			
	*plCurTime=p->Time;

	printf("%lld\t%d\t%lld\t%d\t%s\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->Time,		//< 数据生成时间, 最新订单时间（毫秒）;143025001 表示 14:30:25.001
		p->Symbol);
	
	return 0;
}

int print_MY_TYPE_SSEL2_Transaction(char *buf,char sCodeList[],long *plCurTime)
{
	SSEL2_Transaction   *p=(SSEL2_Transaction *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;
	
	*plCurTime=p->TradeTime;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"tradetime",
			"stockcode");
	}

	printf("%lld\t%d\t%lld\t%d\t%s\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->TradeTime,		//< 成交时间(毫秒), 14302506 表示14:30:25.06
		p->Symbol);
	
	return 0;
}
int print_MY_TYPE_SSEL2_Auction(char *buf,char sCodeList[],long *plCurTime)
{
	SSEL2_Auction   *p=(SSEL2_Auction *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

	*plCurTime=p->Time;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"createtime",
			"stockcode");
	}

	printf("%lld\t%d\t%lld\t%d\t%s\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->Time,		//< 数据生成时间, 143025001 表示 14:30:25.001
		p->Symbol);
	
	return 0;
}
int print_MY_TYPE_SZSEL2_Quotation(char *buf,char sCodeList[],long *plCurTime)
{
	SZSEL2_Quotation   *p=(SZSEL2_Quotation *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;
	
	*plCurTime=p->Time;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"createtime",
			"stockcode");
	}

	printf("%lld\t%d\t%ld\t%d\t%s\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 无数值
		(int)(p->Time%1000000000),//< 数据生成时间YYYYMMDDHHMMSSMMM
		p->Symbol);
	
	return 0;
}
int print_MY_TYPE_SZSEL2_Transaction(char *buf,char sCodeList[],long *plCurTime)
{
	SZSEL2_Transaction   *p=(SZSEL2_Transaction *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

	*plCurTime=p->TradeTime;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"tradetime",
			"stockcode");
	}

	printf("%lld\t%d\t%ld\t%d\t%s\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		(int)(p->TradeTime%1000000000),//成交时间YYYYMMDDHHMMSSMMM
		p->Symbol);
	
	return 0;
}
int print_MY_TYPE_SZSEL2_Order(char *buf,char sCodeList[],long *plCurTime)
{
	SZSEL2_Order   *p=(SZSEL2_Order *)(buf+sizeof(long long));

	//如果代码列表非空且，本代码不在代码列表之内，则不打印
	if(sCodeList[0]&&
		strstr(sCodeList,p->Symbol)==NULL) return 1;

	*plCurTime=p->Time;

	static int iFirstFlag=1;
	if(iFirstFlag){
		iFirstFlag=0;
		printf("%s\t%s\t%s\t%s\t%s\n",
			"picktime",
			"localtime",
			"packtime",
			"ordertime",
			"stockcode");
	}

	printf("%lld\t%d\t%ld\t%d\t%s\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		(int)(p->Time%1000000000),//委托时间YYYYMMDDHHMMSSMMM
		p->Symbol);
	
	return 0;
}
  

int (*comp_time_MY_TYPE)(char *buf,long lBgnTime);
int (*print_MY_TYPE)(char *buf,char sCodeList[],long *plCurTime);

int main(int argc, char *argv[])
{
	int iType=1,i=0,iSkipCnt=0,iFilterCnt=0,iCount=100;
	long lBgnTime=0,lEndTime=999999999999,lCurTime=0,lItemLen=0;
	
	char sInFileName[1024],sCodeStr[1024],sBuffer[10240];
	
	char *buf=sBuffer+sizeof(long long);
	
	FILE *fpIn;	
	
	for (int c; (c = getopt(argc, argv, "i:s:c:b:e:l:t:")) != EOF;){

		switch (c){
		case 'i':strcpy(sInFileName, optarg);	break;
		case 's':iSkipCnt=atoi(optarg);		break;
		case 'c':iCount=atoi(optarg);		break;
		case 'b':lBgnTime=atol(optarg);		break;
		case 'e':lEndTime=atol(optarg);		break;
		case 'l':strcpy(sCodeStr, optarg);	break;
		case 't':
			iType=atoi(optarg);
			if(iType<=0||iType>6) iType=1;
			break;
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-i infile-name ]\n");
			printf("   [-s scipcnt (def=0)]\n");
			printf("   [-c count (def=100,-1=all) ]\n");
			printf("   [-b begin-time (def=0) ]\n");
			printf("   [-e end-tiime (def=99999999999) ]\n");
			printf("   [-t type (1-qh,2-th,3-ah,4-qz,5-tz,6-oz) ]\n");
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
		comp_time_MY_TYPE=comp_time_MY_TYPE_SSEL2_Quotation;
		print_MY_TYPE=print_MY_TYPE_SSEL2_Quotation;
		break;
		case MY_TYPE_SSEL2_Transaction:
		lItemLen=(sizeof(SSEL2_Transaction));
		comp_time_MY_TYPE=comp_time_MY_TYPE_SSEL2_Transaction;
		print_MY_TYPE=print_MY_TYPE_SSEL2_Transaction;
		break;
		case MY_TYPE_SSEL2_Auction:
		lItemLen=(sizeof(SSEL2_Auction));
		comp_time_MY_TYPE=comp_time_MY_TYPE_SSEL2_Auction;
		print_MY_TYPE=print_MY_TYPE_SSEL2_Auction;
		break;
		case MY_TYPE_SZSEL2_Quotation:
		lItemLen=(sizeof(SZSEL2_Quotation));
		comp_time_MY_TYPE=comp_time_MY_TYPE_SZSEL2_Quotation;
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Quotation;
		break;
		case MY_TYPE_SZSEL2_Transaction:
		lItemLen=(sizeof(SZSEL2_Transaction));
		comp_time_MY_TYPE=comp_time_MY_TYPE_SZSEL2_Transaction;
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Transaction;
		break;
		case MY_TYPE_SZSEL2_Order:
		lItemLen=(sizeof(SZSEL2_Order));
		comp_time_MY_TYPE=comp_time_MY_TYPE_SZSEL2_Order;
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Order;
		break;
		default:
		lItemLen=(sizeof(SSEL2_Quotation));
		comp_time_MY_TYPE=comp_time_MY_TYPE_SSEL2_Quotation;
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

		if(comp_time_MY_TYPE(buf,lBgnTime)){
			iSkipCnt++;
			continue;
		}
		
		if(print_MY_TYPE(sBuffer,sCodeStr,&lCurTime)){
			iFilterCnt++;
			continue;
		}
		
		if((++i)>=iCount) break;

		if(iCount>=999999){
			if(lCurTime>lEndTime)
				break;
		}

	}
	
	printf("total skip cnt = %d filter cnt =%d output cnt =%d.\n",iSkipCnt,iFilterCnt,i);

	fclose(fpIn);
	
	return 0;
}

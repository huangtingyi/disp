#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <string>
using namespace std;

#define MY_DATE_CEIL_LONG 1000000000L

#include "wwtiny.h"

#include "gta_supp.h"

typedef struct _filename_set_
{
	string gtaqhName;
	string gtaahName;
	string gtathName;
	string gtaqzName;
	string gtatzName;
	string gtaozName;
	
	string tdfmktName;
	string tdfqueName;
	string tdftraName;
	string tdfordName;
	

	FILE *fpGtaQh;
	FILE *fpGtaAh;
	FILE *fpGtaTh;
	FILE *fpGtaQz;
	FILE *fpGtaTz;
	FILE *fpGtaOz;
	
	FILE *fpTdfMkt;
	FILE *fpTdfQue;
	FILE *fpTdfTra;
	FILE *fpTdfOrd;
	

} FileNameSet;


char sSourcePath[1024],sWorkRoot[1024],sDataDate[15];

FileNameSet MySet;

int OpenFileReadWrite(char sSourcePath[],char sWorkPath[],char sDataDate[])
{
	string strTmp;
	
	strTmp=string(sSourcePath);

	MySet.gtaqhName=	strTmp+"/gta_qh_"+string(sDataDate)+".dat";
	MySet.gtaahName=	strTmp+"/gta_ah_"+string(sDataDate)+".dat";
	MySet.gtathName=	strTmp+"/gta_th_"+string(sDataDate)+".dat";
	MySet.gtaqzName=	strTmp+"/gta_qz_"+string(sDataDate)+".dat";
	MySet.gtatzName=	strTmp+"/gta_tz_"+string(sDataDate)+".dat";
	MySet.gtaozName=	strTmp+"/gta_oz_"+string(sDataDate)+".dat";
		
	if((MySet.fpGtaQh=fopen(MySet.gtaqhName.c_str(),"r"))==NULL)
		{printf("打开文件%s读失败.\n",MySet.gtaqhName.c_str());return -1;}
	if((MySet.fpGtaAh=fopen(MySet.gtaahName.c_str(),"r"))==NULL)  
		{printf("打开文件%s读失败.\n",MySet.gtaahName.c_str());return -1;}
	if((MySet.fpGtaTh=fopen(MySet.gtathName.c_str(),"r"))==NULL)
		{printf("打开文件%s读失败.\n",MySet.gtathName.c_str());return -1;}
	if((MySet.fpGtaQz=fopen(MySet.gtaqzName.c_str(),"r"))==NULL)
		{printf("打开文件%s读失败.\n",MySet.gtaqzName.c_str());return -1;}
	if((MySet.fpGtaTz=fopen(MySet.gtatzName.c_str(),"r"))==NULL)
		{printf("打开文件%s读失败.\n",MySet.gtatzName.c_str());return -1;}
	if((MySet.fpGtaOz=fopen(MySet.gtaozName.c_str(),"r"))==NULL)
		{printf("打开文件%s读失败.\n",MySet.gtaozName.c_str());return -1;}

	strTmp=string(sWorkPath);
	MySet.tdfmktName=	strTmp+"/tdf_mkt_"+string(sDataDate)+".dat";
	MySet.tdfqueName=	strTmp+"/tdf_que_"+string(sDataDate)+".dat";
	MySet.tdftraName=	strTmp+"/tdf_tra_"+string(sDataDate)+".dat";
	MySet.tdfordName=	strTmp+"/tdf_ord_"+string(sDataDate)+".dat"; 

	if((MySet.fpTdfMkt=fopen(MySet.tdfmktName.c_str(),"ab+"))==NULL)
		{printf("打开文件%s写失败.\n",MySet.tdfmktName.c_str());return -1;}
	if((MySet.fpTdfQue=fopen(MySet.tdfqueName.c_str(),"ab+"))==NULL)
		{printf("打开文件%s写失败.\n",MySet.tdfqueName.c_str());return -1;}
	if((MySet.fpTdfTra=fopen(MySet.tdftraName.c_str(),"ab+"))==NULL)
		{printf("打开文件%s写失败.\n",MySet.tdftraName.c_str());return -1;}
	if((MySet.fpTdfOrd=fopen(MySet.tdfordName.c_str(),"ab+"))==NULL)
		{printf("打开文件%s写失败.\n",MySet.tdfordName.c_str());return -1;}
	
	return 0;
}

int WriteSSEL2_Auction2TDF(int nEndTime,FILE *fpIn,FILE *fpMkt)
{
	int iCount=0;
	static int nCurTime=-1;

	char sBuffer[2048];

	//如果时间还没到，则直接返回
	if(nEndTime<nCurTime) return -2;

	SSEL2_Auction *p = (SSEL2_Auction*)(sBuffer+sizeof(long long));
	TDF_MARKET_DATA m;

	while(nCurTime<nEndTime){
		
		
		if(fread((void*)sBuffer,sizeof(SSEL2_Auction)+sizeof(long long),1,fpIn)!=1) break;

		nCurTime=(int)(*(long long*)sBuffer % MY_DATE_CEIL_LONG);
		
		GTA2TDF_SSEL2_AM(p[0],m);
		
		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpMkt)!=1) return -1;
		if(fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,fpMkt)!=1) return -1;
		iCount++;
		
	}
	return iCount;
}
int WriteSSEL2_Quotation2TDF(int nEndTime,FILE *fpIn,FILE *fpMkt,FILE *fpQue)
{

	int iCount=0,ret;
	static int nCurTime=-1;

	char sBuffer[2048];

	//如果时间还没到，则直接返回
	if(nEndTime<nCurTime) return -2;

	SSEL2_Quotation *p = (SSEL2_Quotation*)(sBuffer+sizeof(long long));
	TDF_MARKET_DATA m;
	TDF_ORDER_QUEUE q[2];


	while(nCurTime<nEndTime){
		
		ret=fread((void*)sBuffer,sizeof(SSEL2_Quotation)+sizeof(long long),1,fpIn);

		if(ret!=1) break;

		nCurTime=(int)(*(long long*)sBuffer % MY_DATE_CEIL_LONG);

		GTA2TDF_SSEL2(p[0],m, q[0],q[1]);
		
		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpMkt)!=1) return -1;
		if(fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,fpMkt)!=1) return -1;

		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpQue)!=1) return -1;
		if(fwrite((const void*)&q[0],sizeof(TDF_ORDER_QUEUE),1,fpQue)!=1) return -1;
		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpQue)!=1) return -1;
		if(fwrite((const void*)&q[1],sizeof(TDF_ORDER_QUEUE),1,fpQue)!=1) return -1;
		iCount++;
		
	}
	return iCount;
}
int WriteSSEL2_Transaction2TDF(int nEndTime,FILE *fpIn,FILE *fpTra)
{
	int iCount=0;
	static int nCurTime=-1;

	char sBuffer[2048];

	//如果时间还没到，则直接返回
	if(nEndTime<nCurTime) return -2;

	SSEL2_Transaction *p = (SSEL2_Transaction*)(sBuffer+sizeof(long long));
	TDF_TRANSACTION t;

	while(nCurTime<nEndTime){
		
		
		if(fread((void*)sBuffer,sizeof(SSEL2_Transaction)+sizeof(long long),1,fpIn)!=1) break;

		nCurTime=(int)(*(long long*)sBuffer % MY_DATE_CEIL_LONG);
		
		GTA2TDF_SSEL2_T(p[0],t);
		
		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpTra)!=1) return -1;
		if(fwrite((const void*)&t,sizeof(TDF_TRANSACTION),1,fpTra)!=1) return -1;
		iCount++;
		
	}
	return iCount;
}

int WriteSZSEL2_Order2TDF(int nEndTime,FILE *fpIn,FILE *fpOrd)
{
	int iCount=0;
	static int nCurTime=-1;

	char sBuffer[2048];

	//如果时间还没到，则直接返回
	if(nEndTime<nCurTime) return -2;

	SZSEL2_Order *p = (SZSEL2_Order*)(sBuffer+sizeof(long long));
	TDF_ORDER o;

	while(nCurTime<nEndTime){
		
		
		if(fread((void*)sBuffer,sizeof(SZSEL2_Order)+sizeof(long long),1,fpIn)!=1) break;

		nCurTime=(int)(*(long long*)sBuffer % MY_DATE_CEIL_LONG);
		
		GTA2TDF_SZSEL2_O(p[0],o);
		
		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpOrd)!=1) return -1;
		if(fwrite((const void*)&o,sizeof(TDF_ORDER),1,fpOrd)!=1) return -1;
		iCount++;
		
	}
	return iCount;
}

int WriteSZSEL2_Quotation2TDF(int nEndTime,FILE *fpIn,FILE *fpMkt,FILE *fpQue)
{

	int iCount=0,ret;
	static int nCurTime=-1;

	char sBuffer[2048];

	//如果时间还没到，则直接返回
	if(nEndTime<nCurTime) return -2;

	SZSEL2_Quotation *p = (SZSEL2_Quotation*)(sBuffer+sizeof(long long));
	TDF_MARKET_DATA m;
	TDF_ORDER_QUEUE q[2];


	while(nCurTime<nEndTime){
		
		ret=fread((void*)sBuffer,sizeof(SZSEL2_Quotation)+sizeof(long long),1,fpIn);

		if(ret!=1) break;

		nCurTime=(int)(*(long long*)sBuffer % MY_DATE_CEIL_LONG);

		GTA2TDF_SZSEL2(p[0],m, q[0],q[1]);
		
		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpMkt)!=1) return -1;
		if(fwrite((const void*)&m,sizeof(TDF_MARKET_DATA),1,fpMkt)!=1) return -1;

		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpQue)!=1) return -1;
		if(fwrite((const void*)&q[0],sizeof(TDF_ORDER_QUEUE),1,fpQue)!=1) return -1;
		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpQue)!=1) return -1;
		if(fwrite((const void*)&q[1],sizeof(TDF_ORDER_QUEUE),1,fpQue)!=1) return -1;
		iCount++;
		
	}
	return iCount;
}
int WriteSZSEL2_Transaction2TDF(int nEndTime,FILE *fpIn,FILE *fpTra)
{
	int iCount=0;
	static int nCurTime=-1;

	char sBuffer[2048];

	//如果时间还没到，则直接返回
	if(nEndTime<nCurTime) return -2;

	SZSEL2_Transaction *p = (SZSEL2_Transaction*)(sBuffer+sizeof(long long));
	TDF_TRANSACTION t;

	while(nCurTime<nEndTime){
		
		
		if(fread((void*)sBuffer,sizeof(SZSEL2_Transaction)+sizeof(long long),1,fpIn)!=1) break;

		nCurTime=(int)(*(long long*)sBuffer % MY_DATE_CEIL_LONG);
		
		GTA2TDF_SZSEL2_T(p[0],t);
		
		if(fwrite((const void*)sBuffer,sizeof(long long ),1,fpTra)!=1) return -1;
		if(fwrite((const void*)&t,sizeof(TDF_TRANSACTION),1,fpTra)!=1) return -1;
		iCount++;
		
	}
	return iCount;
}


int main(int argc, char *argv[])
{
	time_t tPreTime,tCurTime,tBeginTime;
	int nCurTime,nPreTime;
	
	strcpy(sSourcePath,	"/stock/work");
	strcpy(sWorkRoot,	"/stock/work");
	
	
	
	GetHostTime(sDataDate);sDataDate[8]=0;

	for (int c; (c = getopt(argc, argv, "i:o:d:?:")) != EOF;){

		switch (c){
		case 'i':
			strcpy(sSourcePath,optarg);
			break;
		case 'o':
			strcpy(sWorkRoot,optarg);
			break;
		case 'd':
			strcpy(sDataDate,optarg);
			break;
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-i source-path ]\n");
			printf("   [-o work-root-name ]\n");
			printf("   [-d data-date (def=today fmt yyyymmdd) ]\n");
			exit(1);
			break;
		}
	}
	
	
	//将输入和输出文件打开
	if(OpenFileReadWrite(sSourcePath,sWorkRoot,sDataDate)<0) return -1;
	
	
	int iCount=0,iAhCnt=0,iQhCnt=0,iThCnt=0,iOzCnt=0,iQzCnt=0,iTzCnt=0;
	nPreTime=nCurTime=90001000;
	
	tBeginTime=tPreTime=tCurTime=tGetHostTime();
	while(1){
		//上海集合竞价
		iCount=WriteSSEL2_Auction2TDF(nCurTime,MySet.fpGtaAh,MySet.fpTdfMkt);
		if(iCount==-1) return -1;
		//if(iCount==0) break;
		if(iCount>=0)	iAhCnt+=iCount;
		
		//上海行情
		iCount=WriteSSEL2_Quotation2TDF(nCurTime,MySet.fpGtaQh,MySet.fpTdfMkt,MySet.fpTdfQue);
		if(iCount==-1) return -1;
		if(iCount>=0)	iQhCnt+=iCount;

		//上海成交
		iCount=WriteSSEL2_Transaction2TDF(nCurTime,MySet.fpGtaTh,MySet.fpTdfTra);
		if(iCount==-1) return -1;
		//if(iCount==0) break;
		if(iCount>=0)	iThCnt+=iCount;

		//深圳委托
		iCount=WriteSZSEL2_Order2TDF(nCurTime,MySet.fpGtaOz,MySet.fpTdfOrd);
		if(iCount==-1) return -1;
		//if(iCount==0) break;
		if(iCount>=0)	iOzCnt+=iCount;

		//深圳行情
		iCount=WriteSZSEL2_Quotation2TDF(nCurTime,MySet.fpGtaQz,MySet.fpTdfMkt,MySet.fpTdfQue);
		if(iCount==-1) return -1;
		if(iCount>=0)	iQzCnt+=iCount;

		//深圳成交
		iCount=WriteSZSEL2_Transaction2TDF(nCurTime,MySet.fpGtaTz,MySet.fpTdfTra);
		if(iCount==-1) return -1;
		//if(iCount==0) break;
		if(iCount>=0)	iTzCnt+=iCount;


		//如果收盘了，就退出吧
		if(nCurTime>151501000) break;
		
		//中午休市时，每10分钟扫描一次
		if(nCurTime>113001000&&nCurTime<125959000)
			nCurTime=iAddMilliSec(nCurTime,1000*60*10);
		else if(nCurTime>150001000)//准收盘了，则1分钟扫描一次
			nCurTime=iAddMilliSec(nCurTime,1000*60);
		else	//正常时段200毫秒扫描一次
			nCurTime=iAddMilliSec(nCurTime,200);
		
		//如果时间超过5分钟，则输出信息
		if((nCurTime-nPreTime)>=500000){
			tCurTime=tGetHostTime();
			printf("time=%d cost=%-3ld\tah=%-8d qh=%-8d th=%-8d oz=%-8d qz=%-8d tz=%-8d.\n",
				nCurTime,tCurTime-tPreTime,iAhCnt,iQhCnt,iThCnt,iOzCnt,iQzCnt,iTzCnt);
			tPreTime=tCurTime;
			nPreTime=nCurTime;
		}
	}
	
	//关闭文件
	fclose(MySet.fpGtaAh);
	fclose(MySet.fpGtaQh);
	fclose(MySet.fpGtaTh);
	fclose(MySet.fpGtaOz);
	fclose(MySet.fpGtaQz);
	fclose(MySet.fpGtaTz);

	fclose(MySet.fpTdfMkt);
	fclose(MySet.fpTdfQue);
	fclose(MySet.fpTdfOrd);
	fclose(MySet.fpTdfTra);

	tCurTime=tGetHostTime();
	printf("time=%d allc=%-3ld\tah=%-8d qh=%-8d th=%-8d oz=%-8d qz=%-8d tz=%-8d.\n",
//	printf("time=%d allc=%-3ld\tah=%-6d\tqh=%-6d\tth=%-6d\toz=%-6d\tqz=%-6d\ttz=%-6d.\n",
		nCurTime,tCurTime-tBeginTime,iAhCnt,iQhCnt,iThCnt,iOzCnt,iQzCnt,iTzCnt);
	
	return 0;
}

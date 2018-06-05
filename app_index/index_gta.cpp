#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "bintree.h"
#include "wwtiny.h"

#include "QTSStruct.h"
#include "index_supp.h"
#include "gta_supp.h"


void  (*GTA2TDF_Q2T)(void *p, TDF_TRANSACTION *pt);
void (*GTA_Quotation2TinyQuotation)(void *p, struct TinyQuotationStruct *po);

void GTA2TDF_QH2T(void *p, TDF_TRANSACTION *pt)
{
	SSEL2_Transaction *pi=(SSEL2_Transaction *)p;

	//char    szCode[32];                 //原始Code
	strcpy(pt->szCode, pi->Symbol);
	//char    szWindCode[32];             //600001.SH
	strcpy(pt->szWindCode, pi->Symbol);
	pt->szWindCode[6] = '.';
	pt->szWindCode[7] = 'S';
	pt->szWindCode[8] = 'H';
	//int     nActionDay;                 //自然日
	pt->nActionDay = int(pi->PacketTimeStamp / 1000000000);
	//int 	nTime;		                //成交时间(HHMMSSmmm)
	pt->nTime = pi->TradeTime;
	//int 	nIndex;		                //成交编号
	pt->nIndex = pi->RecID;
	//__int64		nPrice;		            //成交价格
	pt->nPrice = yuan2percentFen(pi->TradePrice);
	//int 	nVolume;	                //成交数量
	pt->nVolume = pi->TradeVolume;
	//__int64		nTurnover;	            //成交金额
	pt->nTurnover = yuan2percentFen(pi->TradeAmount);
	//int     nBSFlag;                    //买卖方向(买：'B', 卖：'S', 不明：' ')
	switch (pi->BuySellFlag) {
		case 'B':
		case 'b':
			pt->nBSFlag = 'B';
			break;
		case 'S':
		case 's':
			pt->nBSFlag = 'S';
			break;
		default:
			if (pi->BuyRecID > pi->SellRecID)
				pt->nBSFlag = 'B';
			else
				pt->nBSFlag = 'S';
	}
	//char    chOrderKind;                //成交类别
	pt->chOrderKind = 0;
	//char    chFunctionCode;             //成交代码
	pt->chFunctionCode = 0;
	//int	    nAskOrder;	                //叫卖方委托序号
	pt->nAskOrder = int(pi->SellRecID);
	//int	    nBidOrder;	                //叫买方委托序号
	pt->nBidOrder = int(pi->BuyRecID);
}
void  GTA2TDF_QZ2T(void *p, TDF_TRANSACTION *pt)
{
	SZSEL2_Transaction *pi=(SZSEL2_Transaction *)p;

	//char    szWindCode[32];             //600001.SH
	strcpy(pt->szWindCode, pi->Symbol);
	pt->szWindCode[6] = '.';
	pt->szWindCode[7] = 'S';
	pt->szWindCode[8] = 'Z';
	pt->szWindCode[9] = '\0';
	//char    szCode[32];                 //原始Code
	strcpy(pt->szCode, pi->Symbol);
	//int     nActionDay;                 //自然日
	pt->nActionDay = int(pi->TradeTime / 1000000000);
	//int 	nTime;		                //成交时间(HHMMSSmmm)
	pt->nTime = pi->TradeTime % 1000000000;
	//int 	nIndex;		                //成交编号
	pt->nIndex = int(pi->RecID);
	//__int64		nPrice;		            //成交价格
	pt->nPrice = yuan2percentFen(pi->TradePrice);
	//int 	nVolume;	                //成交数量
	pt->nVolume = int(pi->TradeVolume + 0.5);
	//__int64		nTurnover;	            //成交金额
	pt->nTurnover = pt->nPrice * pt->nVolume;
	//char    chOrderKind;                //成交类别
	pt->chOrderKind = '0';
	//char    chFunctionCode;             //成交代码
	switch (pi->TradeType) {
		case '4':
			pt->chFunctionCode = 'C';
			pt->nBSFlag = ' ';
			break;
		default:
			pt->chFunctionCode = '0';
			//int     nBSFlag;                    //买卖方向(买：'B', 卖：'S', 不明：' ')
			if (pi->BuyOrderID > pi->SellOrderID)
				pt->nBSFlag = 'B';
			else
				pt->nBSFlag = 'S';
	}
	//int	    nAskOrder;	                //叫卖方委托序号
	pt->nAskOrder = int(pi->SellOrderID);
	//int	    nBidOrder;	                //叫买方委托序号
	pt->nBidOrder = int(pi->BuyOrderID);
}
void SSEL2_Quotation2TinyQuotation(void *p, struct TinyQuotationStruct *po)
{

	SSEL2_Quotation *pi=(SSEL2_Quotation*)p;

	po->iStockCode=	atoi(pi->Symbol);
	po->nActionDay = int(pi->PacketTimeStamp / 1000000000);
	po->nTime = 	pi->Time;
	po->nPreClose = (int)yuan2percentFen(pi->PreClosePrice);
	po->nOpen = 	(int)yuan2percentFen(pi->OpenPrice);
	po->nHigh = 	(int)yuan2percentFen(pi->HighPrice);
	po->nLow = 	(int)yuan2percentFen(pi->LowPrice);
	po->nMatch = 	(int)yuan2percentFen(pi->LastPrice);
	for (unsigned i = 0; i < 10; ++i) {
		po->nAskVol[i] = (int)pi->SellLevel[i].Volume;
		po->nAskPrice[i] = (int)yuan2percentFen(pi->SellLevel[i].Price);
	}
	for (unsigned i = 0; i < 10; ++i) {
		po->nBidVol[i] = (int)pi->BuyLevel[i].Volume;
		po->nBidPrice[i] = (int)yuan2percentFen(pi->BuyLevel[i].Price);
	}

	po->nNumTrades = int(pi->TotalNo);
	po->iVolume = 		(int8b)pi->TotalVolume;
	po->iTurnover = 	(int8b)(pi->TotalAmount);
	po->nTotalBidVol = 	(int8b)pi->TotalBuyOrderVolume;
	po->nTotalAskVol = 	(int8b)pi->TotalSellOrderVolume;
	po->nWtAvgBidPrice = 	(int)yuan2percentFen(pi->WtAvgBuyPrice);
	po->nWtAvgAskPrice = 	(int)yuan2percentFen(pi->WtAvgSellPrice);
	po->iSamplingFlag=0;
}
void SZSEL2_Quotation2TinyQuotation(void *p, struct TinyQuotationStruct *po)
{

	SZSEL2_Quotation *pi=(SZSEL2_Quotation*)p;

	po->iStockCode=	atoi(pi->Symbol);

	po->nActionDay = int(pi->Time / 1000000000);
	po->nTime = pi->Time % 1000000000;

	po->nPreClose = (int)yuan2percentFen(pi->PreClosePrice);
	po->nOpen = 	(int)yuan2percentFen(pi->OpenPrice);
	po->nHigh = 	(int)yuan2percentFen(pi->HighPrice);
	po->nLow = 	(int)yuan2percentFen(pi->LowPrice);
	po->nMatch = 	(int)yuan2percentFen(pi->LastPrice);

	for (unsigned i = 0; i < LEVEL_TEN; ++i) {
		po->nAskVol[i] = 	(int)(pi->SellLevel[i].Volume);
		po->nAskPrice[i] = 	(int)yuan2percentFen(pi->SellLevel[i].Price);
	}
	for (unsigned i = 0; i < LEVEL_TEN; ++i) {
		po->nBidVol[i] = 	(int)(pi->BuyLevel[i].Volume);
		po->nBidPrice[i] = 	(int)yuan2percentFen(pi->BuyLevel[i].Price);
	}

	po->nNumTrades =int(pi->TotalNo);
	po->iVolume = 	(int8b)(pi->TotalVolume + 0.5);
	po->iTurnover = (int8b)(pi->TotalAmount);
	po->nTotalBidVol = (int8b)(pi->TotalBuyOrderVolume + 0.5);
	po->nTotalAskVol = (int8b)(pi->TotalSellOrderVolume + 0.5);
	po->nWtAvgBidPrice =(int)yuan2percentFen(pi->WtAvgBuyPrice);
	po->nWtAvgAskPrice =(int)yuan2percentFen(pi->WtAvgSellPrice);
	po->iSamplingFlag=0;
}


/**
返回数：
	TAIL_NO_STAT	到达文件末尾，未统计
	WANT_STAT	满足nEndTime0条件，需要统计
	-1	处理错误
**/


int MountTrsData2IndexStatArray(char sFileName[],int nBgnActionDay,
	int nPreT0,int nT0,int nEndTime0,long lItemLen,char sCodeStr[],long *plCurPos)
{
	FILE *fp;
	int iRet;
	char sBuffer[2048];
//	long long *pll=(long long*)sBuffer;
	void *p=(void*)(sBuffer+sizeof(long long));
	TDF_TRANSACTION t;
	struct TinyTransactionStruct tt,*pt;

	LISTHEAD *pSXT;

	long lCurPos=*plCurPos;

	struct IndexStatStruct *pIndexStat;

	if((fp=fopen(sFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sFileName);
		return -1;
	}

	if(fseek(fp,lCurPos,SEEK_SET)<0){
		printf("error fseek file=%s,pos=%ld.\n",sFileName,lCurPos);
		return -1;
	}
	while(1){
		if(fread((void*)sBuffer,lItemLen,1,fp)!=1){
			iRet=MY_TAIL_NO_STAT;
			break;
		}

		lCurPos+=lItemLen;


		//将格式转换为TRANSACTION
		GTA2TDF_Q2T(p,&t);

		if(CodeInCodeStr(t.szCode,sCodeStr)==false) continue;

		if(t.nActionDay!=nBgnActionDay){
			printf("error date file=%s,pos=%ld,actionday=%d,calcdate=%d\n",
				sFileName,lCurPos,t.nActionDay,nBgnActionDay);
			return -1;
		}

		if((pIndexStat=GetIndexStat(atoi(t.szCode),sFileName,
			lCurPos,nBgnActionDay,nPreT0,nT0))==NULL) return -1;

		//过滤掉撤单信息
		if(t.chFunctionCode=='C') continue;

/*		if(iStockCode==2320){
			printf("hello my stock trans.\n");
		}
*/
		//将Transaction数据生成Order数据，添加到Merge结构中
		if(AddTransaction2IndexStat(&t,nT0,pIndexStat)<0){
			printf("error add transaction to index stat file=%s,pos=%ld\n",
			sFileName,lCurPos);
			return -1;
		}

		TDF_TRANSACTION2TinyTransaction(&t,&tt);
/*
		if(tt.nTime==93459180&&tt.nAskOrder==255798){
			printf("find U little cat.\n");
		}
*/
                //建立价格和最大的成交ORDER之间的关系
/*               if(AdjustIndexStatS0(pIndexStat,&tt)<0){
			printf("error add index stat s0 file=%s,pos=%ld\n",
				sFileName,lCurPos);
			return -1;
                }
*/
		//从lCurPos,读到 nT0, 对于大于 nPreT0的部分，加入到
		//S0T，大于nT0的数据加到pS1Head,遇到，遇到大于nEndTime0的数据停止;
//		if(t.nTime<nPreT0) continue;

		//实测表明,取数为 nPreT0<=x<nT0 按这个去取区间
		//如果,t.nTime为收盘时间也归 15:00:00这个区间
		if(MY_BELONG_TRANSACTION_T0(t.nTime,nT0)){
//		if(t.nTime<nT0||(t.nTime==MY_CLOSE_MARKET_TIME&&nT0==MY_CLOSE_MARKET_TIME)){
/***
			填写一下以下4个字段
			int	nAskOrderSeq;	//卖方委托单成交序号，卖成笔数
			int	nBidOrderSeq;	//买方委托单成交序号，买成笔数
			struct TinyOrderStruct *pAskOrder; //叫卖订单指针
			struct TinyOrderStruct *pBidOrder; //叫买订单指针

***/
			if(InitTinyTransactionField(pIndexStat,&tt)<0) return -1;
			pSXT=&(pIndexStat->S0T);
		}
		else
			pSXT=&(pIndexStat->S1T);

		if((pt=(struct TinyTransactionStruct*)malloc(
			sizeof(struct TinyTransactionStruct)))==NULL){
			printf("error malloc TinyTransactionStruct  file=%s,pos=%ld\n",
				sFileName,lCurPos);
			return -1;
		}
		memcpy((void*)pt,(void*)&tt,sizeof(struct TinyTransactionStruct));
		

		Append2List(pSXT,(LIST*)pt);

		//遇到大于nEndTime0的数据停止
		if(t.nTime>nEndTime0){
			iRet=MY_WANT_STAT;
			break;
		}

	}

	fclose(fp);
	*plCurPos=lCurPos;

	return iRet;
}
/*
返回值：
	0 到达文件末尾，或大于nEndTime0,放回
	-1	处理错误
*/
int MountQuotation2IndexStatArray(char sFileName[],int nBgnActionDay,
	int nPreT0,int nT0,int nEndTime0,long lItemLen,char sCodeStr[],long *plCurPos)
{
	int iRet;
	FILE *fp;
	long lCurPos=*plCurPos;
	char sBuffer[2048];
//	long long *pll=(long long*)sBuffer;
	void *p=(SZSEL2_Order*)(sBuffer+sizeof(long long));
	struct TinyQuotationStruct q,*pt;
	struct IndexStatStruct *pIndexStat;

	LISTHEAD *pSXQ;

	if((fp=fopen(sFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sFileName);
		return -1;
	}

	if(fseek(fp,lCurPos,SEEK_SET)<0){
		printf("error fseek file=%s,pos=%ld.\n",sFileName,lCurPos);
		return -1;
	}
	while(1){
		if(fread((void*)sBuffer,lItemLen,1,fp)!=1){
			iRet=MY_TAIL_NO_STAT;
			break;
		}

		lCurPos+=lItemLen;

		GTA_Quotation2TinyQuotation(p,&q);
	
		if(IntCodeInCodeStr(q.iStockCode,sCodeStr)==false) continue;

		if((pIndexStat=GetIndexStat(q.iStockCode,sFileName,
			lCurPos,nBgnActionDay,nPreT0,nT0))==NULL) return -1;

		//从lCurPos,读到 nT0, 对于大于 nPreT0的部分，加入到
		//S0T，大于nT0的数据加到pS1Head,遇到，遇到大于nEndTime0的数据停止;
		if(q.nTime<=nPreT0) continue;

		//实测表明,{对于行情的情况}取数为 nPreT0<x<=nT0 按这个去取区间
		//开盘时候，不取9:30分整点的行情，而取前一笔行情，一般是9点25分的
		if(q.nTime<=nT0&&q.nTime!=MY_OPEN_MARKET_TIME)
			pSXQ=&(pIndexStat->S0Q);
		else	pSXQ=&(pIndexStat->S1Q);

		if((pt=(struct TinyQuotationStruct*)malloc(
			sizeof(struct TinyQuotationStruct)))==NULL){
			printf("error malloc TinyQuotationStruct  file=%s,pos=%ld\n",
				sFileName,lCurPos);
			return -1;
		}
		memcpy((void*)pt,(void*)&q,sizeof(struct TinyQuotationStruct));

		Append2List(pSXQ,(LIST*)pt);

//		遇到大于nEndTime0的数据停止
//		if(q.nTime>=nEndTime0) break;

		//遇到大于nEndTime0的数据停止
		if(q.nTime>=nEndTime0){
			iRet=MY_WANT_STAT;
			break;
		}

	}

	fclose(fp);
	*plCurPos=lCurPos;

	return iRet;
}

/*
返回值：
	0 到达文件末尾，或大于nEndTime0,放回
	-1	处理错误
*/
int MountOrdData2IndexStatArray(char sFileName[],int nBgnActionDay,
	int nPreT0,int nT0,int nEndTime0,long lItemLen,char sCodeStr[],long *plCurPos)
{       
	int iRet;
	FILE *fp;
	long lCurPos=*plCurPos;
	char sBuffer[2048];
//	long long *pll=(long long*)sBuffer;
	SZSEL2_Order *p=(SZSEL2_Order*)(sBuffer+sizeof(long long));
	TDF_ORDER od;
	struct TinyOrderStruct o,*pOrder,*pTemp;
	struct IndexStatStruct *pIndexStat;

	if((fp=fopen(sFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sFileName);
		return -1;
	}

	if(fseek(fp,lCurPos,SEEK_SET)<0){
		printf("error fseek file=%s,pos=%ld.\n",sFileName,lCurPos);
		return -1;
	}
	while(1){
		if(fread((void*)sBuffer,lItemLen,1,fp)!=1){
			iRet=MY_TAIL_NO_STAT;
			break;
		}
		lCurPos+=lItemLen;

		GTA2TDF_SZSEL2_O(p[0],od);
		
		if(CodeInCodeStr(od.szCode,sCodeStr)==false) continue;

		if((pIndexStat=GetIndexStat(atoi(od.szCode),sFileName,
			lCurPos,nBgnActionDay,nPreT0,nT0))==NULL) return -1;

//		if(iStockCode==2320){
//			printf("hello my stock order.\n");
//		}
		if(SearchBin(pIndexStat->M_ORDER,(void*)&od.nOrder,
			data_search_bintree_order_2,(void**)&pOrder)==FOUND){

			pOrder->iCloseFlag=	1;
			pOrder->nOriOrdPrice=	od.nPrice;
			pOrder->nOriOrdVolume=	od.nVolume;
			pOrder->lOriOrdAmnt=	(long)od.nPrice*od.nVolume/100;

			//遇到大于nEndTime0的数据停止
			if(od.nTime>=nEndTime0){
				iRet=MY_WANT_STAT;
				break;    
			}
			continue;
		}

		TDF_ORDER2TinyOrder(&od,&o);

		//将链表头的指针带入
		//o.pNext=&(pIndexStat->PreS0M);
		o.pNext=(struct TinyOrderStruct*)&(pIndexStat->S0O);

		if((pTemp=(struct TinyOrderStruct*)malloc(
			sizeof(struct TinyOrderStruct)))==NULL){
			printf("malloc TinyOrderStruct error.\n");
			return -1;
		}
		memcpy((void*)pTemp,(void*)&o,sizeof(struct TinyOrderStruct));

		if(InsertBin(&(pIndexStat->M_ORDER),(void *)pTemp,
			data_search_bintree_stock_code_order,
			assign_insert_bintree_stock_code_order_e)<0){
			printf("insert bin malloc error th.\n");
			return -1;
		}
		//遇到大于nEndTime0的数据停止
		if(od.nTime>=nEndTime0){
			iRet=MY_WANT_STAT;
			break;
		}
	}

	fclose(fp);
	*plCurPos=lCurPos;

	return iRet;
}

int IsBusyTime(int iTime)
{
	if(iTime<93500000||(iTime>125900000&&iTime<130200000)||
		iTime>145900000) return true;
	return false;
}
int IsStopTime(int iTime)
{
	if((iTime>113500000&&iTime<125900000)) return true;
	return false;
}

int main(int argc, char *argv[])
{
	FILE *fpD31;
	int iIdleWaitMilli=10,iWriteFlag=1;//,iContinueFlag=false;
	int iShBusyDelay=12000,iShDelay=3000,iSzBusyDelay=12000,iSzDelay=2000;
	int nBgnActionDay,nBgnTime,nPreT0,nT0,nEndTime0;
	int iThRes,iTzRes,iQhRes,iQzRes,iOzRes;

	char sCurTime[15],sCalcDate[15],sCalcBgn[15];
	char sDelayStr[256],sCodeStr[512],sD31Name[512],sSourcePath[512],sWorkRoot[512];
	char sGtaQhName[512],sGtaQzName[512],sGtaThName[512],sGtaTzName[512],sGtaOzName[512],sEtfList[512],sEtfPath[512];

	time_t tBeginTime,tEndTime,tDelTime;
	//当前文件处理位置
	long lThCurPos=0,lTzCurPos=0,lOzCurPos=0,lQhCurPos=0,lQzCurPos=0;

//	struct D31ItemStruct XX,*p=&XX;

//	printf("sizof =%ld.\n",sizeof(struct D31ItemStruct));

	GetHostTime(sCurTime);

	//设置参数选项的默认值
	strncpy(sCalcDate,sCurTime,8);sCalcDate[8]=0;
	strcpy(sCalcBgn,sCurTime+8);
	strcpy(sSourcePath,	"/stock/work");
//	iMaxWaitSec=20;
	strcpy(sWorkRoot,	"/stock/work");
	strcpy(sCodeStr,"");
	strcpy(sDelayStr,"");
	strcpy(sEtfList,"510050,510180,510300,510500");
	strcpy(sEtfPath,"../conf/etf");

	//获取命令行参数
	for (int c; (c = getopt(argc, argv, "d:b:m:s:o:e:t:w:l:L:E:?:")) != EOF;){

		switch (c){
		case 'd':strncpy(sCalcDate, optarg,8);sCalcDate[8]=0;	break;
		case 'b':strncpy(sCalcBgn, optarg,6);sCalcBgn[6]=0;	break;
//		case 'm':iMaxWaitSec=atoi(optarg);	break;
		case 's':strcpy(sSourcePath, optarg);	break;
		case 'o':strcpy(sWorkRoot, optarg);	break;
		case 'e':strcpy(sDelayStr,optarg);	break;
		case 't':iIdleWaitMilli=atoi(optarg);	break;
		case 'w':iWriteFlag=atoi(optarg);
			if(iWriteFlag!=1&&iWriteFlag!=2&&iWriteFlag!=3)iWriteFlag=1;	break;
		case 'l':strcpy(sCodeStr, optarg);	break;
		case 'L':strcpy(sEtfList, optarg);	break;
		case 'E':strcpy(sEtfPath, optarg);	break;
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-d calc-date (yyyymmdd =8Bytes def=curdate) ]\n");
			printf("   [-b begin-time (HH24MISS =6Bytes def=curtime) ]\n");
			printf("   [-m max-delay-sec (def=20) ]\n");
			printf("   [-s source-path (def=/stock/work) ]\n");
			printf("   [-o work-root-name (def=/stock/work) ]\n");
			printf("   [-e delay str 'b,i:b:i' ]\n");
			printf("   [-t idlewaitmilli \n");
			printf("   [-w writeflag (1,zbzd 2,ext 3,bin info) \n");
			printf("   [-l codelist (e.g \"000001,603912,002415\") ]\n");
			printf("   [-L etflist (e.g \"510050,510180,510300,510500\") ]\n");
			printf("   [-E etfpath (e.g ../conf/etf) ]\n");
			exit(1);
			break;
		}
	}

	//解析delay串，生成上海和深圳、时延参数
	if(strlen(sDelayStr)!=0){
		char *p0,*p1,*p2;

		if((p0=strchr(sDelayStr,','))==NULL){
			printf("-e =%s 格式非法,正确格式如 '12000,1000:6000,900'.\n",sDelayStr);
			return -1;
		}
		p0++;
		if((p1=strchr(p0,':'))==NULL){
			printf("-e =%s 格式非法,正确格式如 '12000,1000:6000,900'.\n",sDelayStr);
			return -1;
		}
		p1++;
		if((p2=strchr(p1,','))==NULL){
			printf("-e =%s 格式非法,正确格式如 '12000,1000:6000,900'.\n",sDelayStr);
			return -1;
		}
		p2++;
		iShBusyDelay=	atoi(sDelayStr);
		iShDelay=	atoi(p0);
		iSzBusyDelay=	atoi(p1);
		iSzDelay=	atoi(p2);
	}

	//加载ETF参数数据到内存中
	if(InitIndexEtfList(sEtfList,sEtfPath)<0) return -1;

	//生成六个文件名
	sprintf(sGtaThName,"%s/gta_th_%s.dat",sSourcePath,sCalcDate);
	sprintf(sGtaTzName,"%s/gta_tz_%s.dat",sSourcePath,sCalcDate);
	sprintf(sGtaOzName,"%s/gta_oz_%s.dat",sSourcePath,sCalcDate);
	sprintf(sGtaQhName,"%s/gta_qh_%s.dat",sSourcePath,sCalcDate);
	sprintf(sGtaQzName,"%s/gta_qz_%s.dat",sSourcePath,sCalcDate);

	if(iWriteFlag==3)
		sprintf(sD31Name,"%s/d31_t%d_%s.dat",sWorkRoot,iWriteFlag,sCalcDate);
	else	sprintf(sD31Name,"%s/d31_t%d_%s.txt",sWorkRoot,iWriteFlag,sCalcDate);

        if((fpD31=fopen(sD31Name,"w"))==NULL){
                printf("error open file %s to write.\n",sD31Name);
                return -1;
        }

	//得到计算的
	nBgnActionDay=	atoi(sCalcDate);
	nBgnTime=	atoi(sCalcBgn);
	nT0=		nBgnTime*1000;

	if(nBgnTime==93000)
		nPreT0=iAddMilliSec(nT0,-1000*1800);
	else	nPreT0=iAddMilliSec(nT0,-1000*60);

	while(1){

		tBeginTime=tGetHostTime();

		//为了避免订单数据迟到,订单超前取20秒，在收盘最后一笔，手工设置时延
		if(IsBusyTime(nT0)){
			nEndTime0=iAddMilliSec(nT0,iSzBusyDelay+20000);
			if(nT0==MY_CLOSE_MARKET_TIME) nEndTime0=MY_CLOSE_MARKET_TIME+1000000;
		}
		else	nEndTime0=iAddMilliSec(nT0,iSzDelay+20000);

//		if(iContinueFlag==true&&iOzRes==MY_WANT_STAT) goto next_step_tz;
			
		//加载深圳订单数据
		iOzRes=MountOrdData2IndexStatArray(sGtaOzName,nBgnActionDay,nPreT0,nT0, 
			nEndTime0,sizeof(long long)+sizeof(SZSEL2_Order),sCodeStr,&lOzCurPos);
		if(iOzRes<0) return -1;

//next_step_tz:
		if(IsBusyTime(nT0))
			nEndTime0=iAddMilliSec(nT0,iSzBusyDelay);
		else	nEndTime0=iAddMilliSec(nT0,iSzDelay);

//		if(iContinueFlag==true&&iTzRes==MY_WANT_STAT) goto next_step_qz;
			
		//加载深圳交易数据
		GTA2TDF_Q2T=GTA2TDF_QZ2T;
		iTzRes=MountTrsData2IndexStatArray(sGtaTzName,nBgnActionDay,nPreT0,nT0,
			nEndTime0,sizeof(long long)+sizeof(SZSEL2_Transaction),sCodeStr,&lTzCurPos);
		if(iTzRes<0) return -1;
                          
//next_step_qz:
//		if(iContinueFlag==true&&iQzRes==MY_WANT_STAT) goto next_step_th;
		//加载深圳行情数据
		GTA_Quotation2TinyQuotation=SZSEL2_Quotation2TinyQuotation;
		iQzRes=MountQuotation2IndexStatArray(sGtaQzName,nBgnActionDay,nPreT0,nT0,
			nEndTime0,sizeof(long long)+sizeof(SZSEL2_Quotation),sCodeStr,&lQzCurPos);
		if(iQzRes<0) return -1;

//next_step_th:
		
		if(IsBusyTime(nT0))
			nEndTime0=iAddMilliSec(nT0,iShBusyDelay);
		else	nEndTime0=iAddMilliSec(nT0,iShDelay);

//		if(iContinueFlag==true&&iThRes==MY_WANT_STAT) goto next_step_qh;

		//加载上海交易数据
		GTA2TDF_Q2T=GTA2TDF_QH2T;
		iThRes=MountTrsData2IndexStatArray(sGtaThName,nBgnActionDay,nPreT0,nT0,
			nEndTime0,sizeof(long long)+sizeof(SSEL2_Transaction),sCodeStr,&lThCurPos);
		if(iThRes<0) return -1;

//next_step_qh:
//		if(iContinueFlag==true&&iThRes==MY_WANT_STAT) goto next_step_all;
		//加载上海行情数据
		GTA_Quotation2TinyQuotation=SSEL2_Quotation2TinyQuotation;
		iQhRes=MountQuotation2IndexStatArray(sGtaQhName,nBgnActionDay,nPreT0,nT0,
			nEndTime0,sizeof(long long)+sizeof(SSEL2_Quotation),sCodeStr,&lQhCurPos);
		if(iQhRes<0) return -1;

//next_step_all:
		//如果两个成交时间都未达到统计点，
		//死等数据到来，没到来扫描数据一下；
		if(iTzRes!=MY_WANT_STAT&&iThRes!=MY_WANT_STAT){

			int nCurTime=nGetHostCurTime();
						
			//如果时间是午盘时间，休眠
			if(nT0==130001000&&(
				nCurTime>=113000000&&nCurTime<125700000)){
				//休眠三分钟
				usleep(180*1000000);
				continue;
			}
			//只需要考虑实盘环境，回放时候，回放程序已跳过午间休市；
			if(nCurTime>=113000000&&nCurTime<125700000){
				//休眠三分钟
				usleep(180*1000000);
				continue;
			}
			usleep(iIdleWaitMilli*1000);
			continue;
		}

		int nCur=nGetHostCurTime();
		
		printf("hello world tzr=%d,tzp=%ld,thr=%d,thp=%ld,\tcur=%d,ms=%d.\n",
			iTzRes,lTzCurPos,iThRes,lThCurPos,nCur/1000,nCur%1000);

		//做一个循环将D31的数据统计出来
		if(GenD31StatAll()<0) return -1;

		//将D31的数据生成文件
		if(WriteD31StatAll(fpD31,sCodeStr,iWriteFlag)<0) return -1;

//		printf("hello world tp=%d,tc=%d.\n",nPreT0,nT0);

		nPreT0=nT0;

		//如果收盘了，就退出吧
		if(nT0>=150000000) break;

		//中午休市时，直接跳过午间休市，到下午13点00分01秒
		if(nT0>=113000000&&nT0<125959000)
			nT0=130001000;
		else	//正常时段1秒扫描一次
			nT0=iAddMilliSec(nT0,1000);

		//将下一个时间端的ORDER合并到当前树和链表中，
		//如果订单时间超过nT0则将订单保留在原链表中
//		if(MoveS1O2M_ORDERAll(nT0)<0) return -1;

		//将下一个时间段的Transaction合并到当前链表中
		//如果Transaction时间大于nT0则，继续保留在原链表里
		//将下一个时间段的Quotation合并到当前链表中
//		MoveS1X2S0XAll(nPreT0,nT0);

		if(AddPreT0Data2Ready(nPreT0,nT0)<0) return -1;


		//将肯定关闭的ORDER清出内存
		//设置为保留三分钟,只有非忙时的情况再处理

		tDelTime=tGetHostTime();

		int iAskMaxCnt=0,iCnt=0;	//ASK_MAX树的节点数
		int iBidMaxCnt=0;		//BID_MAX树的节点数
		int iS0OCnt=0;			//M_ORDER树的节点数
		int iFreeS0OCnt=0;		//释放掉的S0O数据
/*
		if(!IsBusyTime(nT0)){
			nLastTime=iAddMilliSec(nT0,-1000*60*5);
			pIndexStat=INDEX_HEAD;
			while(pIndexStat!=NULL){

				if(IsStopTime(nT0))
					DeleteCloseOrder(pIndexStat,nLastTime);

				iAskMaxCnt+=	pIndexStat->iAskMaxCnt;
				iBidMaxCnt+=	pIndexStat->iBidMaxCnt;
				iS0OCnt+=	pIndexStat->iS0OCnt;
				iFreeS0OCnt+=	pIndexStat->iFreeS0OCnt;
				iCnt++;

				pIndexStat=pIndexStat->pNext;
			}
		}
*/
		tEndTime=tGetHostTime();

		printf("hello world tp=%d,tc=%d.cost=%ld,%ld\t%d\t%d\t%d\t%d\t%d\n",
			nPreT0,nT0,tEndTime-tBeginTime,tEndTime-tDelTime,
			iCnt,iAskMaxCnt,iBidMaxCnt,iS0OCnt,iFreeS0OCnt);

	}

	fclose(fpD31);

	printf("hello world.\n");
	return 0;
}

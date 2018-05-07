
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include "wwtiny.h"
#include "QTSDataType.h"
#include "QTSStruct.h"
#include "QTSDataFieldDefine.h"

#include "gta_supp.h"

#define MY_TYPE_SSEL2_Quotation		1
#define MY_TYPE_SSEL2_Transaction	2
#define MY_TYPE_SSEL2_Auction		3
#define MY_TYPE_SZSEL2_Quotation	4
#define MY_TYPE_SZSEL2_Transaction	5
#define MY_TYPE_SZSEL2_Order		6

#define MY_TYPE_TDF_MKT			'M'
#define MY_TYPE_TDF_TRA			'T'
#define MY_TYPE_TDF_ORD			'O'
#define MY_TYPE_TDF_QUE			'Q'


#define MY_DATE_CEIL_LONG 1000000000L

int64_t my_yuan2percentFen(const double yuan)
{
	return int64_t((yuan + 0.00005) * 10000);
}
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


	sprintf(outbuf,"%lld\t%d\t%lld\t%d\t%s\t%d\t%s\
\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->Time,		//< 数据生成时间, 最新订单时间（毫秒）;143025001 表示 14:30:25.001
		p->Symbol,
		iDiffnTime((int)lTmpTime,(int)p->Time),
		p->QuotationFlag,				//第一个续行
    		(int)my_yuan2percentFen(p->PreClosePrice),         ///< 昨收价,
    		(int)my_yuan2percentFen(p->OpenPrice),             ///< 开盘价,
    		(int)my_yuan2percentFen(p->HighPrice),             ///< 最高价,
    		(int)my_yuan2percentFen(p->LowPrice),              ///< 最低价,
    		(int)my_yuan2percentFen(p->LastPrice),             ///< 现价,
    		(int)my_yuan2percentFen(p->ClosePrice),            ///< 今收盘价,
		p->TradeStatus,    				///< 当前品种交易状态,
		p->SecurityPhaseTag,   				///< 当前品种状态
		(int)p->TotalNo,                		///< 成交笔数,
		(int)p->TotalVolume,           			///< 成交总量, 股票：股),权证：份),债券：手
    		(int)my_yuan2percentFen(p->TotalAmount),           ///< 成交总额, （元）
		(int)p->TotalBuyOrderVolume,    		///< 委托买入总量, 股票：股),权证：份),债券：手
    		(int)my_yuan2percentFen(p->WtAvgBuyPrice),         ///< 加权平均委买价, （元）
    		(int)my_yuan2percentFen(p->BondWtAvgBuyPrice),     ///< 债券加权平均委买价, （元）
		(int)p->TotalSellOrderVolume,			///< 委托卖出总量,
    		(int)my_yuan2percentFen(p->WtAvgSellPrice),        ///< 加权平均委卖价, （元）
    		(int)my_yuan2percentFen(p->BondWtAvgSellPrice),    ///< 债券加权平均委卖价,
    		(int)my_yuan2percentFen(p->IOPV),                  ///< ETF 净值估值,
		p->ETFBuyNo,					///< ETF 申购笔数,	第二个续行
		(int)p->ETFBuyVolume, 			        ///< ETF 申购量,
    		(int)my_yuan2percentFen(p->ETFBuyAmount),          ///< ETF 申购额,
    		p->ETFSellNo,             			///< ETF 赎回笔数,
    		(int)p->ETFSellVolume,          		///< ETF 赎回量,
    		(int)my_yuan2percentFen(p->ETFSellAmount),         ///< ETF 赎回额,
    		(int)my_yuan2percentFen(p->YTM),                   ///< 债券到期收益率,
    		(int)p->TotalWarrantExecVol,    		///< 权证执行的总数量,
    		(int)my_yuan2percentFen(p->WarrantDownLimit),      ///< 权证跌停价格, （元）
    		(int)my_yuan2percentFen(p->WarrantUpLimit),        ///< 权证涨停价格, （元）
    		p->WithdrawBuyNo,          			///< 买入撤单笔数,
    		(int)p->WithdrawBuyVolume,      		///< 买入撤单量,
    		(int)my_yuan2percentFen(p->WithdrawBuyAmount),      ///< 买入撤单额,
    		p->WithdrawSellNo,				///< 卖出撤单笔数,
    		(int)p->WithdrawSellVolume,			///< 卖出撤单量,
    		(int)my_yuan2percentFen(p->WithdrawSellAmount),     ///< 卖出撤单额,
    		p->TotalBuyNo,             ///< 买入总笔数,
    		p->TotalSellNo,            ///< 卖出总笔数,
    		p->MaxBuyDuration,         ///< 买入成交最大等待时间,
    		p->MaxSellDuration,        ///< 卖出成交最大等待时间,
    		p->BuyOrderNo,             ///< 买方委托价位数,
    		p->SellOrderNo,            ///< 卖方委托价位数,		第三个续行
    		(int)p->SellLevelNo,	   ///< 卖盘价位数量, 10档行情，不足时补空 第四个续行
    		(int)my_yuan2percentFen(p->SellPrice01),
    		(int)p->SellVolume01,
    		(int)p->TotalSellOrderNo01,
    		(int)my_yuan2percentFen(p->SellPrice02),
    		(int)p->SellVolume02,
    		(int)p->TotalSellOrderNo02,
    		(int)my_yuan2percentFen(p->SellPrice03),
    		(int)p->SellVolume03,
    		(int)p->TotalSellOrderNo03,
    		(int)my_yuan2percentFen(p->SellPrice04),
    		(int)p->SellVolume04,
    		(int)p->TotalSellOrderNo04,
    		(int)my_yuan2percentFen(p->SellPrice05),
    		(int)p->SellVolume05,
    		(int)p->TotalSellOrderNo05,		//第五个续行
    		(int)my_yuan2percentFen(p->SellPrice06),
    		(int)p->SellVolume06,
    		(int)p->TotalSellOrderNo06,
    		(int)my_yuan2percentFen(p->SellPrice07),
    		(int)p->SellVolume07,
    		(int)p->TotalSellOrderNo07,
    		(int)my_yuan2percentFen(p->SellPrice08),
    		(int)p->SellVolume08,
    		(int)p->TotalSellOrderNo08,
    		(int)my_yuan2percentFen(p->SellPrice09),
    		(int)p->SellVolume09,
    		(int)p->TotalSellOrderNo09,
    		(int)my_yuan2percentFen(p->SellPrice10),
    		(int)p->SellVolume10,
    		(int)p->TotalSellOrderNo10,		//第六个续行
    		(int)p->SellLevelQueueNo01,
    		(int)p->SellLevelQueue[0],
    		(int)p->BuyLevelNo,			//第七个续行
    		(int)my_yuan2percentFen(p->BuyPrice01),
    		(int)p->BuyVolume01,
    		(int)p->TotalBuyOrderNo01,
    		(int)my_yuan2percentFen(p->BuyPrice02),
    		(int)p->BuyVolume02,
    		(int)p->TotalBuyOrderNo02,
    		(int)my_yuan2percentFen(p->BuyPrice03),
    		(int)p->BuyVolume03,
    		(int)p->TotalBuyOrderNo03,
    		(int)my_yuan2percentFen(p->BuyPrice04),
    		(int)p->BuyVolume04,
    		(int)p->TotalBuyOrderNo04,
    		(int)my_yuan2percentFen(p->BuyPrice05),
    		(int)p->BuyVolume05,
    		(int)p->TotalBuyOrderNo05,		//第八个续行
    		(int)my_yuan2percentFen(p->BuyPrice06),
    		(int)p->BuyVolume06,
    		(int)p->TotalBuyOrderNo06,
    		(int)my_yuan2percentFen(p->BuyPrice07),
    		(int)p->BuyVolume07,
    		(int)p->TotalBuyOrderNo07,
    		(int)my_yuan2percentFen(p->BuyPrice08),
    		(int)p->BuyVolume08,
    		(int)p->TotalBuyOrderNo08,
    		(int)my_yuan2percentFen(p->BuyPrice09),
    		(int)p->BuyVolume09,
    		(int)p->TotalBuyOrderNo09,
    		(int)my_yuan2percentFen(p->BuyPrice10),
    		(int)p->BuyVolume10,
    		(int)p->TotalBuyOrderNo10,    		//第九个续行
    		(int)p->BuyLevelQueueNo01,
    		(int)p->BuyLevelQueue[0]
		);

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

	sprintf(outbuf,"%lld\t%d\t%lld\t%d\t%s\t%d\
\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%c\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->TradeTime,		//< 成交时间(毫秒), 14302506 表示14:30:25.06
		p->Symbol,
		iDiffnTime((int)lTmpTime,(int)p->TradeTime),//第二行开始
		p->QuotationFlag,  			///< 1=上海南汇机房行情源；2=深圳福永机房行情源
		(int)p->RecID,                  	///< 业务索引, 从 1 开始，按 TradeChannel连续
		(int)p->TradeChannel,           	///< 成交通道,
		(int)my_yuan2percentFen(p->TradePrice),	///< 成交价格,
		(int)p->TradeVolume,            	///< 成交数量, 股票：股,权证：份,债券：张
		(int)my_yuan2percentFen(p->TradeAmount),///< 成交金额,
		(int)p->BuyRecID,               	///< 买方订单号,
		(int)p->SellRecID,              	///< 卖方订单号,
		p->BuySellFlag            		///< 内外盘标志, B – 外盘,主动买,S – 内盘,主动卖,N – 未知
		);

	return 0;
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

	sprintf(outbuf,"%lld\t%d\t%lld\t%d\t%s\t%d\t%s\t%d\t%d\t%d\t%c\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		p->PacketTimeStamp,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		p->Time,		//< 数据生成时间, 143025001 表示 14:30:25.001
		p->Symbol,
		iDiffnTime((int)lTmpTime,(int)p->Time),
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

	sprintf(outbuf,"%lld\t%d\t%ld\t%d\t%s\t%d\t%s\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\
\t%d\t%d\t%d\t%d\t%d\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 无数值
		(int)(p->Time%MY_DATE_CEIL_LONG),//< 数据生成时间YYYYMMDDHHMMSSMMM
		p->Symbol,
		iDiffnTime((int)lTmpTime,(int)(p->Time%MY_DATE_CEIL_LONG)),
		p->QuotationFlag,				//第一个续行
    		(int)my_yuan2percentFen(p->PreClosePrice),
    		(int)my_yuan2percentFen(p->OpenPrice),
    		(int)my_yuan2percentFen(p->LastPrice),
    		(int)my_yuan2percentFen(p->HighPrice),
    		(int)my_yuan2percentFen(p->LowPrice),
    		(int)my_yuan2percentFen(p->PriceUpLimit),
    		(int)my_yuan2percentFen(p->PriceDownLimit),
    		(int)my_yuan2percentFen(p->PriceUpdown1),
    		(int)my_yuan2percentFen(p->PriceUpdown2),
    		(int)p->TotalNo,
    		(int)my_yuan2percentFen(p->TotalVolume),
    		(int)my_yuan2percentFen(p->TotalAmount),
    		(int)my_yuan2percentFen(p->ClosePrice),
    		"0",//p->SecurityPhaseTag,
    		(int)my_yuan2percentFen(p->PERatio1),
    		(int)my_yuan2percentFen(p->NAV),
    		(int)my_yuan2percentFen(p->PERatio2),
    		(int)my_yuan2percentFen(p->IOPV),
    		(int)my_yuan2percentFen(p->PremiumRate),
    		(int)my_yuan2percentFen(p->TotalSellOrderVolume),
    		(int)my_yuan2percentFen(p->WtAvgSellPrice),	//第二个续行
    		(int)p->SellLevelNo,	   ///< 卖盘价位数量, 10档行情，不足时补空 第三个续行
    		(int)my_yuan2percentFen(p->SellPrice01),
    		(int)p->SellVolume01,
    		(int)p->TotalSellOrderNo01,
    		(int)my_yuan2percentFen(p->SellPrice02),
    		(int)p->SellVolume02,
    		(int)p->TotalSellOrderNo02,
    		(int)my_yuan2percentFen(p->SellPrice03),
    		(int)p->SellVolume03,
    		(int)p->TotalSellOrderNo03,
    		(int)my_yuan2percentFen(p->SellPrice04),
    		(int)p->SellVolume04,
    		(int)p->TotalSellOrderNo04,
    		(int)my_yuan2percentFen(p->SellPrice05),
    		(int)p->SellVolume05,
    		(int)p->TotalSellOrderNo05,		//第四个续行
    		(int)my_yuan2percentFen(p->SellPrice06),
    		(int)p->SellVolume06,
    		(int)p->TotalSellOrderNo06,
    		(int)my_yuan2percentFen(p->SellPrice07),
    		(int)p->SellVolume07,
    		(int)p->TotalSellOrderNo07,
    		(int)my_yuan2percentFen(p->SellPrice08),
    		(int)p->SellVolume08,
    		(int)p->TotalSellOrderNo08,
    		(int)my_yuan2percentFen(p->SellPrice09),
    		(int)p->SellVolume09,
    		(int)p->TotalSellOrderNo09,
    		(int)my_yuan2percentFen(p->SellPrice10),
    		(int)p->SellVolume10,
    		(int)p->TotalSellOrderNo10,		//第五个续行
    		(int)p->SellLevelQueueNo01,
    		(int)p->SellLevelQueue[0],
    		(int)my_yuan2percentFen(p->TotalBuyOrderVolume),
    		(int)my_yuan2percentFen(p->WtAvgBuyPrice),
    		(int)p->BuyLevelNo,			//第六个续行
    		(int)my_yuan2percentFen(p->BuyPrice01),
    		(int)p->BuyVolume01,
    		(int)p->TotalBuyOrderNo01,
    		(int)my_yuan2percentFen(p->BuyPrice02),
    		(int)p->BuyVolume02,
    		(int)p->TotalBuyOrderNo02,
    		(int)my_yuan2percentFen(p->BuyPrice03),
    		(int)p->BuyVolume03,
    		(int)p->TotalBuyOrderNo03,
    		(int)my_yuan2percentFen(p->BuyPrice04),
    		(int)p->BuyVolume04,
    		(int)p->TotalBuyOrderNo04,
    		(int)my_yuan2percentFen(p->BuyPrice05),
    		(int)p->BuyVolume05,
    		(int)p->TotalBuyOrderNo05,		//第七个续行
    		(int)my_yuan2percentFen(p->BuyPrice06),
    		(int)p->BuyVolume06,
    		(int)p->TotalBuyOrderNo06,
    		(int)my_yuan2percentFen(p->BuyPrice07),
    		(int)p->BuyVolume07,
    		(int)p->TotalBuyOrderNo07,
    		(int)my_yuan2percentFen(p->BuyPrice08),
    		(int)p->BuyVolume08,
    		(int)p->TotalBuyOrderNo08,
    		(int)my_yuan2percentFen(p->BuyPrice09),
    		(int)p->BuyVolume09,
    		(int)p->TotalBuyOrderNo09,
    		(int)my_yuan2percentFen(p->BuyPrice10),
    		(int)p->BuyVolume10,
    		(int)p->TotalBuyOrderNo10,    		//第八个续行
    		(int)p->BuyLevelQueueNo01,
    		(int)p->BuyLevelQueue[0],
    		(int)my_yuan2percentFen(p->WtAvgRate),
    		(int)my_yuan2percentFen(p->WtAvgRateUpdown),
    		(int)my_yuan2percentFen(p->PreWtAvgRate)
		);

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

	sprintf(outbuf,"%lld\t%d\t%ld\t%d\t%s\t%d\
\t%s\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%c\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		(int)(p->TradeTime%MY_DATE_CEIL_LONG),//成交时间YYYYMMDDHHMMSSMMM
		p->Symbol,
		iDiffnTime((int)lTmpTime,(int)(p->TradeTime%MY_DATE_CEIL_LONG)),//第二行开始
		p->QuotationFlag,  			///< 1=上海南汇机房行情源；2=深圳福永机房行情源
		(int)p->SetID,                  	///< 证券集代号
		(int)p->RecID,           		///< 消息记录号 从 1 开始计数，同一频道连续
		(int)p->BuyOrderID,                  	///< 买方委托索引
		(int)p->SellOrderID,           		///< 卖方委托索引
		p->SymbolSource,			///< 证券代码源
		(int)my_yuan2percentFen(p->TradePrice),	///< 成交价格,
		(int)my_yuan2percentFen(p->TradeVolume),///< 成交金额,
		p->TradeType            		///< 成交类别：4=撤销，主动或自动撤单执行报告；F=成交，成交执行报告
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


	sprintf(outbuf,"%lld\t%d\t%ld\t%d\t%s\t%d\
\t%s\t%d\t%d\t%s\t%d\t%d\t%c\t%c\n",
		*(long long *)buf,	//picktime
		p->LocalTimeStamp,	//< 数据接收时间HHMMSSMMM
		99999999999999999,	//< 数据包头时间YYYYMMDDHHMMSSMMM
		(int)(p->Time%MY_DATE_CEIL_LONG),//委托时间YYYYMMDDHHMMSSMMM
		p->Symbol,
		iDiffnTime((int)lTmpTime,(int)(p->Time%MY_DATE_CEIL_LONG)),//第二行开始
		p->QuotationFlag,  			///< 1=上海南汇机房行情源；2=深圳福永机房行情源
		(int)p->SetID,                  	///< 证券集代号
		(int)p->RecID,           		///< 消息记录号 从 1 开始计数，同一频道连续
		p->SymbolSource,			///< 证券代码源
		(int)my_yuan2percentFen(p->OrderPrice),	///< 委托价格,4位小数
		(int)my_yuan2percentFen(p->OrderVolume),///< 委托数量,2位小数
		p->OrderCode,				///< 买卖方向：1=买 2=卖 G=借入 F=出借
		p->OrderType				///< 订单类别：1=市价 2=限价 U=本方最优
		);

	return 0;
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
		iDiffnTime((int)lTmpTime,(int)p->nTime),//第一行继续
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
		iDiffnTime((int)lTmpTime,(int)p->nTime),//第二行开始
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
		iDiffnTime((int)lTmpTime,(int)p->nTime),//第二行开始
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
		iDiffnTime((int)lTmpTime,(int)p->nTime),//第二行开始
		(char)p->nSide,			//买卖方向('B':Bid 'A':Ask)
		(int)p->nPrice,         	//委托价格
		(int)p->nOrders,		//订单数量
		(int)p->nABItems,		//明细个数
		(int)p->nABVolume[0]		//订单明细
		);

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
			//增加TDF的四种类型
			switch (optarg[0]){
				case 'm':case 'M':iType='M';break;
				case 't':case 'T':iType='T';break;
				case 'o':case 'O':iType='O';break;
				case 'q':case 'Q':iType='Q';break;
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
			printf("   [-t type (1-qh,2-th,3-ah,4-qz,5-tz,6-oz,mtoq|MTOQ=>mkt,trs,ord,que) ]\n");
			printf("   [-d delay sec (-e,-l multi-code effect) ]\n");
			printf("   [-l codelist (e.g \"000001,603912,002415\") ]\n");
			printf("   [-f time_flag (def=1,1-createtime,2-picktime,3-localtime,4-packtime) ]\n");
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
		case MY_TYPE_TDF_MKT:
		lItemLen=(sizeof(TDF_MARKET_DATA));
		print_MY_TYPE=print_MY_TYPE_TDF_MKT;
		break;
		case MY_TYPE_TDF_TRA:
		lItemLen=(sizeof(TDF_TRANSACTION));
		print_MY_TYPE=print_MY_TYPE_TDF_TRA;
		break;
		case MY_TYPE_TDF_ORD:
		lItemLen=(sizeof(TDF_ORDER));
		print_MY_TYPE=print_MY_TYPE_TDF_ORD;
		break;
		case MY_TYPE_TDF_QUE:
		lItemLen=(sizeof(TDF_ORDER_QUEUE));
		print_MY_TYPE=print_MY_TYPE_TDF_QUE;
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

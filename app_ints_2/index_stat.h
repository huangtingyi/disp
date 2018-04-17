#ifndef __INDEX_STAT_H__
#define __INDEX_STAT_H__

#define MAX_LEVEL_CNT	10

//本结构336个字节
struct D31IndexItemStruct
{
	struct D31IndexItemStruct *pNext;
	int	iStockCode;	//股票代码
	int	nActionDay;	//委托日期(YYYYMMDD)
	int	nTime;		//委托时间(HHMMSSmmm)
	int8b	alBuyAmount[MAX_LEVEL_CNT];	//主买额度，单位（分）
	int	aiBuyVolume[MAX_LEVEL_CNT];	//主买量，单位（手）
	int	alBuyOrderNo[MAX_LEVEL_CNT];	//主买笔数，单位（笔）
	int8b	alSaleAmount[MAX_LEVEL_CNT];	//主卖额度，单位（分）
	int	aiSaleVolume[MAX_LEVEL_CNT];	//主卖量，单位（手）
	int	aiSaleOrderNo[MAX_LEVEL_CNT];	//主卖笔数，单位（笔）
};

//每个结构40个字节，16+4*6 = 40
struct TinyTransactionStruct
{
	struct TinyTransactionStruct *pNext;		//下一个交易数据
	int	nActionDay;	//成交日期(YYYYMMDD)
	int	nTime;		//成交时间(HHMMSSmmm)
	int	iStockCode;	//证券代码
	int	nPrice;		//成交价格, nPrice==>nPrice,成交价格不会很大用int够用。
	int 	nVolume;	//成交量
	int	nAskOrder;	//卖方委托序号
	int	nBidOrder;	//买方委托序号
	int     nBSFlag;        //买卖方向(买：'B', 卖：'S', 不明：' ')
};
//每个结构40个字节，16+4*6 = 40
struct TinyOrderStruct
{
	struct TinyOrderStruct *pNext;
	int	nActionDay;	//委托日期(YYYYMMDD)
	int	nTime;		//委托时间(HHMMSSmmm)
	int	iStockCode;	//证券代码
	int	nPrice;		//委托价格, nPrice==>nPrice，成交价格不会很大用int够用。
	int 	nVolume;	//成交量
	int 	nOrder;	        //委托号
	int     nBroker;	//经济商编号
	int     nBSFlag;  	//委托代码('B','S','C')
};
struct PriceOrderStruct
{
	int nPrice;
	int nOrder;
};
struct IndexStatStruct
{
	struct IndexStatStruct *pNext;	//下一支股票的数据
	int iStockCode;
	int nActionDay;
	int nPreT0;
	int nT0;
	int iS0Cnt;		//S0链表的大小
	int iPreS0Cnt;		//在S0链表中，发现的小于nPreT0的记录数
/*
	int nAskDownPrice;	//卖出最低成交价,单位（分）
	int nBidUpPrice;	//买入最高成交价,单位（分）
	int nAskMaxOrder;	//卖出最大订单号，对应被动盘最低价
	int nBidMaxOrder;	//买入最大订单号，对应被动盘最高价
*/	
	BINTREE *ASK_MAX;	//以成卖出交价为索引，成交价和最大ASK_ORDER的关系
	BINTREE *BID_MAX;	//以成买入交价为索引，成交价和最大BID_ORDER的关系
	struct D31IndexItemStruct I;
	BINTREE *M_ORDER;			//合并订单数据，按ORDER_ID索引
	BINTREE *R_ORDER;			//真实订单数据，按ORDER_ID索引
	struct TinyOrderStruct PreS0M;		//之前的合并订单数据
	struct TinyOrderStruct PreS0R;		//之前的实际订单数据
	struct TinyOrderStruct *pS0M;		//合并订单数据列表
	struct TinyOrderStruct *pS0R;		//实际订单数据列表
	struct TinyTransactionStruct *pS0Head;	//存放(T0-1,T0]的交易数据
	struct TinyTransactionStruct *pS1Head;  //存放(T0,之后的交易数据};
};

#define MY_GET_MILLI_SEC(x)	(x%1000)
#define MY_GET_SEC(x)		((x%100000)/1000)
#define MY_GET_MIM(x)		((x%10000000)/100000)
#define MY_GET_HOUR(x)		(x/10000000)

#endif

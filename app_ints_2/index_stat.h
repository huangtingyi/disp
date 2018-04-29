#ifndef __INDEX_STAT_H__
#define __INDEX_STAT_H__

#define MAX_LEVEL_CNT		10
#define MAX_JMP_LEVEL_CNT 	3

//逐笔和逐单统计信息
struct D31IndexItemStruct
{
//	struct D31IndexItemStruct *pNext;
//	int	nActionDay;	//委托日期(YYYYMMDD)
//	int	nTime;		//委托时间(HHMMSSmmm)
	int8b	alBidAmount[MAX_LEVEL_CNT];	//主买额度，单位（分）
	int	aiBidVolume[MAX_LEVEL_CNT];	//主买量，单位（手）
	int	aiBidOrderNum[MAX_LEVEL_CNT];	//主买笔数，单位（笔）
	int8b	alAskAmount[MAX_LEVEL_CNT];	//主卖额度，单位（分）
	int	aiAskVolume[MAX_LEVEL_CNT];	//主卖量，单位（手）
	int	aiAskOrderNum[MAX_LEVEL_CNT];	//主卖笔数，单位（笔）
};

//右侧扩展统计信息
struct D31IndexExtStruct
{
	//上半部分指标开始
	int	nTenBidVolume;			//十档买量（手）
	int	nTenAskVolume;			//十档卖量（手）
	int8b	lTenBidAmnt;			//十档买额（分）
	int8b	lTenAskAmnt;			//十档卖额（分）
	int	nTotalBidVolume;		//叫买总量（手）
	int	nTotalAskVolume;		//叫卖总量（手）
	int8b	lTotalBidAmnt;			//叫买总额（分）
	int8b	lTotalAskAmnt;			//叫卖总额（分）
	int	nWtAvgBidPrice;			//加权平均叫买价（分）
	int	nWtAvgAskPrice;			//加权平均叫卖价（分）
	//下半部分指标开始
	int	nLastClose;			//昨收盘价
	int	nCurPrice;			//最新价
	int8b	lAvgTotalBidAmnt;		//平均叫买总额（当日平均，分）
	int8b	lAvgTotalAskAmnt;		//平均叫卖总额（当日平均，分）
	int8b	alBidAmount[MAX_JMP_LEVEL_CNT];	//跳买额度，单位（分）
	int8b	alAskAmount[MAX_JMP_LEVEL_CNT];	//跳卖额度，单位（分）
};

//每个结构40个字节，16+4*6 = 40
struct TinyTransactionStruct
{
	struct TinyTransactionStruct *pNext;		//下一个交易数据
	int	nActionDay;	//成交日期(YYYYMMDD)
	int	nTime;		//成交时间(HHMMSSmmm)
	int	iStockCode;	//证券代码
	int	nPrice;		//成交价格(1手，100股，单位分) nPrice==>nPrice,成交价格不会很大用int够用。
	int 	nVolume;	//成交量(手数*100=股数)
	int	nAskOrder;	//卖方委托序号
	int	nBidOrder;	//买方委托序号
	int     nBSFlag;        //买卖方向(买：'B', 卖：'S', 不明：' ')
	
	int	nAskOrderSeq;	//卖方委托单成交序号，卖成笔数
	int	nBidOrderSeq;	//买方委托单成交序号，买成笔数
	int	nAskJmpSeq;	//跳卖序列
	int	nBidJmpSeq;	//跳买序列

	struct TinyOrderStruct *pAskOrder;
	struct TinyOrderStruct *pBidOrder;
};
//每个结构40个字节，16+4*6 = 40
struct TinyOrderStruct
{
	struct TinyOrderStruct *pNext;
//	struct TinyOrderStruct *pTmpNext;
	int	nActionDay;	//委托日期(YYYYMMDD)
	int	nTime;		//委托时间(HHMMSSmmm)
	int	iStockCode;	//证券代码
	int	nPrice;		//委托价格(1手，100股，单位分) nPrice==>nPrice，成交价格不会很大用int够用。
	int 	nVolume;	//成交量(手数*100=股数)
	int 	nOrder;	        //委托号
	int     nBroker;	//经济商编号
	int     nBSFlag;  	//委托代码('B','S','C')
	
	int	iCloseFlag;	//订单关闭标志，0未关闭，1已关闭
	int	nOrderSeq;	//成交序列号
	long	lOrderAmnt;	//成交金额
	
	int	nLastPrice;	//最新价
	int	nAskJmpSeq;	//跳卖序列
	int	nBidJmpSeq;	//跳买序列

	int	nOriOrdPrice;	//原始委托价格
	int	nOriOrdVolume;	//原始委托量
	long	lOriOrdAmnt;	//原始委托金额
	
};
struct TinyQuotationStruct
{
	struct TinyQuotationStruct *pNext;
	int	nActionDay;	//日期(YYYYMMDD)
	int	nTime;		//行情时间(HHMMSSmmm)
	int	iStockCode;	//证券代码
    	int nPreClose;		//前收盘价
    	int nOpen;		//开盘价
    	int nHigh;		//最高价
    	int nLow;		//最低价
    	int nMatch;		//最新价
    	int nAskPrice[10];	//申卖价
    	int nAskVol[10];	//申卖量
    	int nBidPrice[10];	//申买价
    	int nBidVol[10];	//申买量
	int nNumTrades;		//成交笔数
    	int8b	iVolume;	//成交总量
    	int8b	iTurnover;	//成交总金额
    	int8b	nTotalBidVol;	//委托买入总量
    	int8b	nTotalAskVol;	//委托卖出总量
    	int	nWtAvgBidPrice;	//加权平均委买价格
    	int	nWtAvgAskPrice;	//加权平均委卖价格
};

struct PriceOrderStruct
{
	int nPrice;
	int nOrder;
	int nOrderTime;
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
	
	int iAskMaxCnt;		//ASK_MAX树的节点数
	int iBidMaxCnt;		//BID_MAX树的节点数
	int iS0OCnt;		//M_ORDER树的节点数
	int iFreeS0OCnt;	//释放掉的S0D数据
	
	int	iQuotationCnt;		//行情笔数
	int8b	lAddupTotalBidAmnt;	//每笔行情叫买总量的累计
	int8b	lAddupTotalAskAmnt;	//每笔行情叫卖总量的累计
	
	BINTREE *ASK_MAX;		//以成卖出交价为索引，成交价和最大ASK_ORDER的关系
	BINTREE *BID_MAX;		//以成买入交价为索引，成交价和最大BID_ORDER的关系
	struct D31IndexItemStruct Zd;	//逐单统计数据
	struct D31IndexItemStruct Zb;	//逐笔统计数据
	struct D31IndexExtStruct  Ex;	//扩展信息
	BINTREE *M_ORDER;		//合并订单数据，按ORDER_ID索引
	LISTHEAD S0O;	//存放(~,T0]所有的M_ORDER中的订单列表
	LISTHEAD S1O;	//存放[T0,~)之后的ORDER数据}
	LISTHEAD S0T;	//存放[T0-1,T0),{当T0==15:00:00 时，为 [14:59,15:00]}的交易数据
	LISTHEAD S1T;	//存放[T0,之后的交易数据}
	LISTHEAD S0Q;	//存放[T0-1,T0),{当T0==15:00:00 时，为 [14:59,15:00]}的行情数据
	LISTHEAD S1Q;	//存放[T0,之后的行情数据}
};

#endif

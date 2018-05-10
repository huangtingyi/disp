#pragma once
#include <string>
#include <vector>

#include <chrono>
#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

#pragma  pack(push)
#pragma pack(1)

struct IpDomain
{
	string ip;
	bool fromDomain = false; //原先配置文件里是否为域名
};

extern unsigned int g_iPort;
extern string g_strPassword;
extern string g_strUsername;
extern string password;
extern int st_interface;
extern IpDomain g_strServer;

struct TDF_MARKET_DATA2
{
	char szWindCode[32]; //600001.SH
	char szCode[32]; //原始Code
	int nActionDay; //业务发生日(自然日)
	int nTradingDay; //交易日
	int nTime; //时间(HHMMSSmmm)
	int nStatus; //状态
	int64_t nPreClose; //前收盘价
	int64_t nOpen; //开盘价
	int64_t nHigh; //最高价
	int64_t nLow; //最低价
	int64_t nMatch; //最新价
	int64_t nAskPrice[10]; //申卖价
	int64_t nAskVol[10]; //申卖量
	int64_t nBidPrice[10]; //申买价
	int64_t nBidVol[10]; //申买量
	int nNumTrades; //成交笔数
	int64_t iVolume; //成交总量
	int64_t iTurnover; //成交总金额
	int64_t nTotalBidVol; //委托买入总量
	int64_t nTotalAskVol; //委托卖出总量
	int64_t nWeightedAvgBidPrice; //加权平均委买价格
	int64_t nWeightedAvgAskPrice; //加权平均委卖价格
	int nIOPV; //IOPV净值估值
	int nYieldToMaturity; //到期收益率
	int64_t nHighLimited; //涨停价
	int64_t nLowLimited; //跌停价
	char chPrefix[4]; //证券信息前缀
	int nSyl1; //市盈率1
	int nSyl2; //市盈率2
	int nSD2; //升跌2（对比上一笔）
};

struct TDF_TRANSACTION2
{
	char szWindCode[32]; //600001.SH
	char szCode[32]; //原始Code
	int32_t nActionDay; //自然日
	int32_t nTime; //成交时间(HHMMSSmmm)
	int32_t nIndex; //成交编号
	int64_t nPrice; //成交价格
	int nVolume; //成交数量
	int64_t nTurnover; //成交金额
	int nBSFlag; //买卖方向(买：'B', 卖：'S', 不明：' ')
	char chOrderKind; //成交类别
	char chFunctionCode; //成交代码
	int nAskOrder; //叫卖方委托序号
	int nBidOrder; //叫买方委托序号
};

struct TDF_ORDER_QUEUE2
{
	char szWindCode[32]; //600001.SH
	char szCode[32]; //原始Code
	int nActionDay; //自然日
	int nTime; //时间(HHMMSSmmm)
	int nSide; //买卖方向('B':Bid 'A':Ask)
	int64_t nPrice; //委托价格
	int nOrders; //订单数量
	int nABItems; //明细个数
	int nABVolume[200]; //订单明细
	//int getABItems();
};

struct TDF_ORDER2
{
	char szWindCode[32]; //600001.SH
	char szCode[32]; //原始Code
	int nActionDay; //委托日期(YYMMDD)
	int nTime; //委托时间(HHMMSSmmm)
	int nOrder; //委托号
	int64_t nPrice; //委托价格
	int nVolume; //委托数量
	char chOrderKind; //委托类别
	char chFunctionCode; //委托代码('B','S','C')
	int nBroker; //经纪商编码
	char chStatus; //委托状态
	char chFlag; //标志
};

#ifndef SYMBOL_LEN
#define SYMBOL_LEN 40
#endif
/// 上交所L2指数
struct SSEL2_Index2
{
	int Time; ///< 数据生成时间(毫秒), 143025000 表示 14:30:25000
	int TradeTime; ///< 成交时间(毫秒),
	char Symbol[SYMBOL_LEN]; ///< 证券代码,
	double TotalAmount; ///< 成交总额,
	double HighPrice; ///< 最高价,
	double LowPrice; ///< 最低价,
	double LastPrice; ///< 现价,
	unsigned long long TotalVolume; ///< 成交总量, 手
};

/// 深交所L2指数行情
struct SZSEL2_Index2
{
	long long Time; ///< 数据生成时间YYYYMMDDHHMMSSMMM
	char Symbol[SYMBOL_LEN]; ///< 证券代码
	double HighPrice; ///< 最高指数,6位小数
	double LowPrice; ///< 最低指数,6位小数
	double LastPrice; ///< 最新指数,6位小数
	double TotalAmount; ///< 参与计算相应指数的成交金额,4位小数
	unsigned long long TotalNo; ///< 参与计算相应指数的成交笔数
	double TotalVolume; ///< 参与计算相应指数的交易数量，2位小数
};

enum MarketInterfaceError
{
	M_SUCCESS = (int16_t) 0,
	ERR_CONNECT = (int16_t) 1, //网络连接错误
	ERR_LOGIN = (int16_t) 2, //登录失败
};

#pragma pack(pop)


typedef void (*RecvHandleM)(const TDF_MARKET_DATA2 *msg);
typedef void (*RecvHandleT)(const TDF_TRANSACTION2 *msg);
typedef void (*RecvHandleQ)(const TDF_ORDER_QUEUE2 *msg);
typedef void (*RecvHandleO)(const TDF_ORDER2 *msg);
//typedef void( *RecvHandleIdxH)(const SSEL2_Index2 *msg);
//typedef void( *RecvHandleIdxZ)(const SZSEL2_Index2 *msg);
typedef void (*RecvHandleE)(int16_t errCode, const char *errmsg);

//返回值：true:成功 false:失败
//bool initIdxH(RecvHandleIdxH handleIH);
//bool initIdxZ(RecvHandleIdxZ handleIZ);
//errmsg须准备好256字节空间
int16_t m_connect(char *errmsg);
int16_t login(char *errmsg);
//登录成功后再调用。获取可订阅的证券代码。codes须事先准备好8000*4byte的空间。返回值：证券代码数量
int32_t getCodes(int32_t *codes);
//一旦调用此函数，则只会发送相关证券代码的行情。若从未调用此函数，则全代码发送。addReduce:1增加,0减少
void addReduceCode(int8_t addReduce, int32_t *codes, int32_t size);
void subscribe(bool marketdata, bool transaction, bool orderqueue, bool order);
//void subscribeIH(int8_t subOrCancel);//1订阅沪指，0取消订阅
//void subscribeIZ(int8_t subOrCancel);//1订阅深指，0取消订阅
void replay(bool marketdata, bool transaction, bool orderqueue, bool order, uint32_t timeBegin, uint32_t timeEnd, uint32_t date);
//m_connect, login, RecvHandleE反馈错误时后调用此函数。之后用户可再次m_connect并login
//不推荐在RecvHandleE的堆栈下启动此函数
void marketInterfaceClose();

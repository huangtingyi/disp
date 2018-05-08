#include <cstring>
#include <cstdint>

#include "wwtiny.h"
#include "gta_supp.h"

struct LimitUpDownStruct LIMIT[MAX_STOCK_CODE];

void InitLimitArray()
{
	for(int i=0;i<MAX_STOCK_CODE;i++){
		LIMIT[i].WarrantDownLimit=0.0;
		LIMIT[i].WarrantUpLimit=0.0;
	}
}

char GTA2TDF_MARKET_STATUS(const char *TradeStatus, const char *SecurityPhaseTag) {
	//null 正常交易

	//0 首日上市
	//1 增发新股
	//2 上网定价发行
	//3 上网竞价发行
	//'A' 交易节休市

	//'B' 整天停牌
	if (SecurityPhaseTag[0] == 'P') return 'B';
	//'C' 全天收市
	//'D' 暂停交易

	//'E'(START)启动交易盘
	if (!strcmp(TradeStatus, "START")) return 'E';
	//'F'(PRETR)盘前处理
	if (!strcmp(TradeStatus, "PRETR")) return 'F';
	//'G'(DEL)不可恢复交易的熔断阶段（上交所的N）
	if (!strcmp(TradeStatus, "DEL")) return 'G';
	//'H'(HOLIDAY)放假

	//'I'(OCALL)开市集合竞价
	if (!strcmp(TradeStatus, "OCALL")) return 'I';
	//'J'(ICALL)盘中集合竞价
	if (!strcmp(TradeStatus, "ICALL")) return 'J';
	//'K'(OPOBB)开市订单簿平衡前期
	if (!strcmp(TradeStatus, "OPOBB")) return 'K';
	//'L'(IPOBB)盘中订单簿平衡前期
	if (!strcmp(TradeStatus, "IPOBB")) return 'L';
	//'M(OOBB ) 开市订单簿平衡
	if (!strcmp(TradeStatus, "OOBB")) return 'M';
	//'N'(IOBB)盘中订单簿平衡
	if (!strcmp(TradeStatus, "IOBB")) return 'N';
	//'O'(TRADE)连续撮合
	if (!strcmp(TradeStatus, "TRADE")) return 'O';
	//'P'(BREAK)休市
	if (!strcmp(TradeStatus, "BREAK")) return 'P';
	//'Q'(VOLA)波动性中断
	if (!strcmp(TradeStatus, "VOLA")) return 'Q';
	//'R'(BETW)交易间
	if (!strcmp(TradeStatus, "BETW")) return 'R';
	//'S'(NOTRD)非交易服务支持
	if (!strcmp(TradeStatus, "NOTRD")) return 'S';
	//'T'(FCALL)固定价格集合竞价
	if (!strcmp(TradeStatus, "FCALL")) return 'T';
	//'U'(POSTR)盘后处理
	if (!strcmp(TradeStatus, "POSTR")) return 'U';
	//'V'(ENDTR)结束交易
	if (!strcmp(TradeStatus, "ENDTR")) return 'V';
	//'W'(HALT)暂停
	if (!strcmp(TradeStatus, "HALT")) return 'W';
	//'X'(SUSP)停牌
	if (!strcmp(TradeStatus, "SUSP")) return 'X';
	//'Y'(ADD)新增产品
	if (!strcmp(TradeStatus, "ADD")) return 'Y';
	//'Z'(DEL)可删除的产品
	if (!strcmp(TradeStatus, "DEL")) return 'Z';
	//'d' 集合竞价阶段结束到连续竞价阶段开始之前的时段（如有）
	if (SecurityPhaseTag[0] == 'D') return 'd';
	//'q' 可恢复交易的熔断时段(上交所的M)
	if (SecurityPhaseTag[0] == 'M') return 'q';

	return 0;
};

void GTA2TDF_SSEL2(const SSEL2_Quotation &src, TDF_MARKET_DATA &m, TDF_ORDER_QUEUE &qB, TDF_ORDER_QUEUE &qS) {
	//char        szWindCode[32];         //600001.SH
	strcpy(m.szWindCode, src.Symbol);
	m.szWindCode[6] = '.';
	m.szWindCode[7] = 'S';
	m.szWindCode[8] = 'H';
	m.szWindCode[9] = '\0';
	//char        szCode[32];             //原始Code
	strcpy(m.szCode, src.Symbol);
	//int         nActionDay;             //业务发生日(自然日)
	m.nActionDay = int(src.PacketTimeStamp / 1000000000);
	//int         nTradingDay;            //交易日
	m.nTradingDay = m.nActionDay;
	//int			 nTime;					//时间(HHMMSSmmm)
	m.nTime = src.Time;
	//int			 nStatus;				//状态
	m.nStatus = GTA2TDF_MARKET_STATUS(src.TradeStatus, src.SecurityPhaseTag);
	//__int64 nPreClose;					//前收盘价
	m.nPreClose = yuan2percentFen(src.PreClosePrice);
	//__int64 nOpen;						//开盘价
	m.nOpen = yuan2percentFen(src.OpenPrice);
	//__int64 nHigh;						//最高价
	m.nHigh = yuan2percentFen(src.HighPrice);
	//__int64 nLow;						//最低价
	m.nLow = yuan2percentFen(src.LowPrice);
	//__int64 nMatch;						//最新价
	m.nMatch = yuan2percentFen(src.LastPrice);
	for (unsigned i = 0; i < 10; ++i) {
		//__int64 nAskVol[10];				//申卖量
		m.nAskVol[i] = src.SellLevel[i].Volume;
		m.nAskPrice[i] = yuan2percentFen(src.SellLevel[i].Price);
	}
	for (unsigned i = 0; i < 10; ++i) {
		//__int64 nBidVol[10];				//申买量
		m.nBidVol[i] = src.BuyLevel[i].Volume;
		m.nBidPrice[i] = yuan2percentFen(src.BuyLevel[i].Price);		
	}

	//int nNumTrades;						//成交笔数
	m.nNumTrades = int(src.TotalNo);
	//__int64		 iVolume;				//成交总量
	m.iVolume = src.TotalVolume;
	//__int64		 iTurnover;				//成交总金额
	m.iTurnover = (__int64)(src.TotalAmount);
	//__int64		 nTotalBidVol;			//委托买入总量
	m.nTotalBidVol = src.TotalBuyOrderVolume;
	//__int64		 nTotalAskVol;			//委托卖出总量
	m.nTotalAskVol = src.TotalSellOrderVolume;
	//__int64 nWeightedAvgBidPrice;		//加权平均委买价格
	m.nWeightedAvgBidPrice = yuan2percentFen(src.WtAvgBuyPrice);
	//__int64 nWeightedAvgAskPrice;		//加权平均委卖价格
	m.nWeightedAvgAskPrice = yuan2percentFen(src.WtAvgSellPrice);
	//int			 nIOPV;					//IOPV净值估值
	//int			 nYieldToMaturity;		//到期收益率

	//__int64 nHighLimited;				//涨停价
	//__int64 nLowLimited;				//跌停价
	//g_LimitPriceMgr.getLimitPrices(m.nHighLimited, m.nLowLimited, src.Symbol);
	
	m.nHighLimited=		yuan2percentFen(src.WarrantUpLimit);
	m.nLowLimited=		yuan2percentFen(src.WarrantDownLimit);

	//char		 chPrefix[4];			//证券信息前缀
	//int			 nSyl1;					//市盈率1
	//int			 nSyl2;					//市盈率2
	//int			 nSD2;					//升跌2（对比上一笔）

	//char    szWindCode[32]; //600001.SH
	strcpy(qB.szWindCode, m.szWindCode);
	strcpy(qS.szWindCode, m.szWindCode);
	//char    szCode[32];     //原始Code
	strcpy(qB.szCode, src.Symbol);
	strcpy(qS.szCode, src.Symbol);
	//int     nActionDay;     //自然日
	qB.nActionDay = m.nActionDay;
	qS.nActionDay = m.nActionDay;
	//int 	nTime;			//时间(HHMMSSmmm)
	qB.nTime = m.nTime;
	qS.nTime = m.nTime;
	//int     nSide;			//买卖方向('B':Bid 'A':Ask)
	qB.nSide = 'B';
	qS.nSide = 'A';
	//__int64		nPrice;		//委托价格
	qB.nPrice = yuan2percentFen(src.BuyPrice01);
	qS.nPrice = yuan2percentFen(src.SellPrice01);
	//int 	nOrders;		//订单数量
	qB.nOrders = src.BuyLevelQueueNo01;
	qS.nOrders = src.SellLevelQueueNo01;
	//int 	nABItems;		//明细个数
	//int 	nABVolume[200];	//订单明细
	qB.nABItems=qS.nABItems=0;
	for (unsigned i = 0; i < ORDER_LEVEL_FIFTY; ++i) {
		if (!(qB.nABVolume[i] = src.BuyLevelQueue[i])) {
			qB.nABItems = i;
			break;
		}
	}
	for (unsigned i = 0; i < ORDER_LEVEL_FIFTY; ++i) {
		if (!(qS.nABVolume[i] = src.SellLevelQueue[i])) {
			qS.nABItems = i;
			break;
		}
	}
};

void GTA2TDF_SSEL2_T(const SSEL2_Transaction &src, TDF_TRANSACTION &t) {
	//char    szCode[32];                 //原始Code
	strcpy(t.szCode, src.Symbol);
	//char    szWindCode[32];             //600001.SH 
	strcpy(t.szWindCode, src.Symbol);
	t.szWindCode[6] = '.';
	t.szWindCode[7] = 'S';
	t.szWindCode[8] = 'H';
	//int     nActionDay;                 //自然日
	t.nActionDay = int(src.PacketTimeStamp / 1000000000);
	//int 	nTime;		                //成交时间(HHMMSSmmm)
	t.nTime = src.TradeTime;
	//int 	nIndex;		                //成交编号
	t.nIndex = src.RecID;
	//__int64		nPrice;		            //成交价格
	t.nPrice = yuan2percentFen(src.TradePrice);
	//int 	nVolume;	                //成交数量
	t.nVolume = src.TradeVolume;
	//__int64		nTurnover;	            //成交金额
	t.nTurnover = yuan2percentFen(src.TradeAmount);
	//int     nBSFlag;                    //买卖方向(买：'B', 卖：'S', 不明：' ')
	switch (src.BuySellFlag) {
		case 'B':
		case 'b':
			t.nBSFlag = 'B';
			break;
		case 'S':
		case 's':
			t.nBSFlag = 'S';
			break;
		default:
			if (src.BuyRecID > src.SellRecID)
				t.nBSFlag = 'B';
			else
				t.nBSFlag = 'S';
	}
	//char    chOrderKind;                //成交类别
	t.chOrderKind = 0;
	//char    chFunctionCode;             //成交代码
	t.chFunctionCode = 0;
	//int	    nAskOrder;	                //叫卖方委托序号
	t.nAskOrder = int(src.SellRecID);
	//int	    nBidOrder;	                //叫买方委托序号
	t.nBidOrder = int(src.BuyRecID);
}

void GTA2TDF_SZSEL2(const SZSEL2_Quotation &src, TDF_MARKET_DATA &m, TDF_ORDER_QUEUE &qB, TDF_ORDER_QUEUE &qS) {
	//char        szWindCode[32];         //600001.SH
	strcpy(m.szWindCode, src.Symbol);
	m.szWindCode[6] = '.';
	m.szWindCode[7] = 'S';
	m.szWindCode[8] = 'Z';
	m.szWindCode[9] = '\0';
	//char        szCode[32];             //原始Code
	strcpy(m.szCode, src.Symbol);
	//int         nActionDay;             //业务发生日(自然日)
	m.nActionDay = int(src.Time / 1000000000);
	//int         nTradingDay;            //交易日
	m.nTradingDay = m.nActionDay;
	//int			 nTime;					//时间(HHMMSSmmm)
	m.nTime = src.Time % 1000000000;
	//int			 nStatus;				//状态		
	//< 第 1 位：0=正常状态,1=全天停牌"
	if (src.SecurityPhaseTag[1] == '1') {
		m.nStatus = 'B';
	}
	else {
		//第 0 位：
		switch (src.SecurityPhaseTag[0]) {
			//T = 连续
		case 'T':
			m.nStatus = 'O';
			break;
			//S = 启动（开市前）
		case 'S':
			m.nStatus = 'E';
			break;
			//O = 开盘集合竞价
		case 'O':
			m.nStatus = 'I';
			break;
			//B = 休市
		case 'B':
			m.nStatus = 'P';
			break;
			//C = 收盘集合竞价

			//E = 已闭市
		case 'E':
			m.nStatus = 'V';
			break;
			//H = 临时停牌
		case 'H':
			m.nStatus = 'W';
			break;
			//A = 盘后交易
		case 'A':
			m.nStatus = 'U';
			break;
			//V = 波动性中断
		case 'V':
			m.nStatus = 'Q';
			break;
		}
	}
	//__int64 nPreClose;					//前收盘价
	m.nPreClose = yuan2percentFen(src.PreClosePrice);
	//__int64 nOpen;						//开盘价
	m.nOpen = yuan2percentFen(src.OpenPrice);
	//__int64 nHigh;						//最高价
	m.nHigh = yuan2percentFen(src.HighPrice);
	//__int64 nLow;						//最低价
	m.nLow = yuan2percentFen(src.LowPrice);
	//__int64 nMatch;						//最新价
	m.nMatch = yuan2percentFen(src.LastPrice);

	for (unsigned i = 0; i < LEVEL_TEN; ++i) {
		//完全复制获取数据		
		m.nAskVol[i] = (__int64)(src.SellLevel[i].Volume);
		m.nAskPrice[i] = yuan2percentFen(src.SellLevel[i].Price);
	}
	for (unsigned i = 0; i < LEVEL_TEN; ++i) {
		//完全复制获取数据__int64 nBidVol[10];			//申买量
		m.nBidVol[i] = (__int64)(src.BuyLevel[i].Volume);
		m.nBidPrice[i] = yuan2percentFen(src.BuyLevel[i].Price);
	}

	//int nNumTrades;						//成交笔数
	m.nNumTrades = int(src.TotalNo);
	//__int64		 iVolume;				//成交总量
	m.iVolume = (__int64)(src.TotalVolume + 0.5);
	//__int64		 iTurnover;				//成交总金额
	m.iTurnover = (__int64)(src.TotalAmount);
	//__int64		 nTotalBidVol;			//委托买入总量
	m.nTotalBidVol = (__int64)(src.TotalBuyOrderVolume + 0.5);
	//__int64		 nTotalAskVol;			//委托卖出总量
	m.nTotalAskVol = (__int64)(src.TotalSellOrderVolume + 0.5);
	//__int64 nWeightedAvgBidPrice;		//加权平均委买价格
	m.nWeightedAvgBidPrice = yuan2percentFen(src.WtAvgBuyPrice);
	//__int64 nWeightedAvgAskPrice;		//加权平均委卖价格
	m.nWeightedAvgAskPrice = yuan2percentFen(src.WtAvgSellPrice);
	//int			 nIOPV;					//IOPV净值估值
	//int			 nYieldToMaturity;		//到期收益率
	//__int64 nHighLimited;				//涨停价
	m.nHighLimited = yuan2percentFen(src.PriceUpLimit);
	//__int64 nLowLimited;				//跌停价
	m.nLowLimited = yuan2percentFen(src.PriceDownLimit);
	//char		 chPrefix[4];			//证券信息前缀

	//int			 nSyl1;					//市盈率1
	m.nSyl1 = int((src.PERatio1 + 0.005) * 100);
	//int			 nSyl2;					//市盈率2
	m.nSyl2 = int((src.PERatio2 + 0.005) * 100);
	//int			 nSD2;					//升跌2（对比上一笔）
	m.nSD2 = int((src.PriceUpdown2 + 0.005) * 100);

	//char    szWindCode[32];             //600001.SH
	strcpy(qB.szWindCode, m.szWindCode);
	strcpy(qS.szWindCode, m.szWindCode);
	//char    szCode[32];                 //原始Code
	strcpy(qB.szCode, src.Symbol);
	strcpy(qS.szCode, src.Symbol);
	//int     nActionDay;                 //自然日
	qB.nActionDay = m.nActionDay;
	qS.nActionDay = m.nActionDay;
	//int 	nTime;		                //成交时间(HHMMSSmmm)
	qB.nTime = m.nTime;
	qS.nTime = m.nTime;
	//int     nSide;			//买卖方向('B':Bid 'A':Ask)
	qB.nSide = 'B';
	qS.nSide = 'A';
	//__int64		nPrice;		//委托价格
	qB.nPrice = yuan2percentFen(src.BuyPrice01);
	qS.nPrice = yuan2percentFen(src.SellPrice01);
	//int 	nOrders;		//订单数量
	qB.nOrders = src.BuyLevelQueueNo01;
	qS.nOrders = src.SellLevelQueueNo01;
	//int 	nABItems;		//明细个数
	//int 	nABVolume[200];	//订单明细
	qB.nABItems=qS.nABItems=0;
	for (unsigned i = 0; i < ORDER_LEVEL_FIFTY; ++i) {
		if (!(qB.nABVolume[i] = int(src.BuyLevelQueue[i]))) {
			qB.nABItems = i;
			break;
		}
	}
	for (unsigned i = 0; i < ORDER_LEVEL_FIFTY; ++i) {
		if (!(qS.nABVolume[i] = int(src.SellLevelQueue[i]))) {
			qS.nABItems = i;
			break;
		}
	}
}

void  GTA2TDF_SZSEL2_T(const SZSEL2_Transaction &src, TDF_TRANSACTION &t) {
	//char    szWindCode[32];             //600001.SH
	strcpy(t.szWindCode, src.Symbol);
	t.szWindCode[6] = '.';
	t.szWindCode[7] = 'S';
	t.szWindCode[8] = 'Z';
	t.szWindCode[9] = '\0';
	//char    szCode[32];                 //原始Code
	strcpy(t.szCode, src.Symbol);
	//int     nActionDay;                 //自然日
	t.nActionDay = int(src.TradeTime / 1000000000);
	//int 	nTime;		                //成交时间(HHMMSSmmm)
	t.nTime = src.TradeTime % 1000000000;
	//int 	nIndex;		                //成交编号
	t.nIndex = int(src.RecID);
	//__int64		nPrice;		            //成交价格
	t.nPrice = yuan2percentFen(src.TradePrice);
	//int 	nVolume;	                //成交数量
	t.nVolume = int(src.TradeVolume + 0.5);
	//__int64		nTurnover;	            //成交金额
	t.nTurnover = t.nPrice * t.nVolume;
	//char    chOrderKind;                //成交类别
	t.chOrderKind = '0';
	//char    chFunctionCode;             //成交代码
	switch (src.TradeType) {
		case '4':
			t.chFunctionCode = 'C';
			t.nBSFlag = ' ';
			break;
		default:
			t.chFunctionCode = '0';
			//int     nBSFlag;                    //买卖方向(买：'B', 卖：'S', 不明：' ')
			if (src.BuyOrderID > src.SellOrderID)
				t.nBSFlag = 'B';
			else
				t.nBSFlag = 'S';
	}
	//int	    nAskOrder;	                //叫卖方委托序号
	t.nAskOrder = int(src.SellOrderID);
	//int	    nBidOrder;	                //叫买方委托序号
	t.nBidOrder = int(src.BuyOrderID);
}

void GTA2TDF_SZSEL2_O(const SZSEL2_Order &src, TDF_ORDER &o) {
	//char    szWindCode[32]; //600001.SH 
	strcpy(o.szWindCode, src.Symbol);
	o.szWindCode[6] = '.';
	o.szWindCode[7] = 'S';
	o.szWindCode[8] = 'Z';
	o.szWindCode[9] = '\0';
	//char    szCode[32];     //原始Code
	strcpy(o.szCode, src.Symbol);
	//int 	nActionDay;	    //委托日期(YYMMDD)
	o.nActionDay = int(src.Time / 100000000);
	//int 	nTime;			//委托时间(HHMMSSmmm)
	o.nTime = src.Time % 1000000000;
	//int 	nOrder;	        //委托号
	o.nOrder = int(src.RecID);
	//__int64		nPrice;		//委托价格
	o.nPrice = yuan2percentFen(src.OrderPrice);
	//int 	nVolume;		//委托数量
	o.nVolume = int(src.OrderVolume);
	//char    chOrderKind;	//委托类别
	o.chOrderKind = src.OrderType;
	switch (src.OrderType) {
	case '2':
		o.chOrderKind = '0';
		break;
	default:
		o.chOrderKind = src.OrderType;
	}
	//char    chFunctionCode;	//委托代码('B','S','C')
	switch (src.OrderCode) {
	case '1':
		o.chFunctionCode = 'B';
		break;
	case '2':
		o.chFunctionCode = 'S';
		break;
	default:
		o.chFunctionCode = src.OrderCode;
	}
	//int     nBroker;		//经纪商编码
	//char    chStatus;		//委托状态
	//char	chFlag;			//标志
}

void  GTA2TDF_SSEL2_AM(const SSEL2_Auction &src, TDF_MARKET_DATA &m) {
	
	bzero((void*)&m,sizeof(TDF_MARKET_DATA));

	//char        szWindCode[32];         //600001.SH
	strcpy(m.szWindCode, src.Symbol);
	m.szWindCode[6] = '.';
	m.szWindCode[7] = 'S';
	m.szWindCode[8] = 'H';
	m.szWindCode[9] = '\0';
	//char        szCode[32];             //原始Code
	strcpy(m.szCode, src.Symbol);
	//int         nActionDay;             //业务发生日(自然日)
	m.nActionDay = int(src.PacketTimeStamp / 1000000000);
	//int         nTradingDay;            //交易日
	m.nTradingDay = m.nActionDay;
	//int			 nTime;					//时间(HHMMSSmmm)
	m.nTime = src.Time % 1000000000;
	//int			 nStatus;				//状态
	//__int64 nPreClose;					//前收盘价
	//__int64 nOpen;						//开盘价
	//__int64 nHigh;						//最高价
	//__int64 nLow;						//最低价
	//__int64 nMatch;						//最新价

	//__int64 nAskVol[10];				//申卖量
	m.nBidVol[0] = m.nAskVol[0] = src.AuctionVolume;
	//__int64 nAskPrice[10];				//申卖价
	m.nBidPrice[0] = m.nAskPrice[0] = yuan2percentFen(src.OpenPrice);
	switch (src.Side) {
	case '1':
		m.nBidVol[1] = src.LeaveVolume;
		break;
	case '2':
		m.nAskVol[1] = src.LeaveVolume;
		break;
	}
}

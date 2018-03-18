#include "QTSStruct.h"
#include "GtaQh2csv.h"

GtaQh2csv::GtaQh2csv(const string &c)
	:Gta2csv(c) {}

void GtaQh2csv::getFileName(string &strFileName)/*override*/ {
	strFileName = "Quotation.csv";
}

void GtaQh2csv::getHeaders(string &strHeaders)/*override*/ {
	strHeaders = "接收,行情源,包头,生成,证券,昨收,开盘,最高,最低,现价,今收,交易状态,品种状态,成交笔数,成交总量,成交总额,委买总量,加权委买价,债券加权委买价,委卖总量,加权委卖价,债券加权平均委卖价\
,IOPV,ETF申购笔数,ETF申购量,ETF申购额,ETF赎回笔数,ETF赎回量,ETF赎回额,YTM,权证执行的总数量,权证跌停价,权证涨停价\
,买入撤单笔,买入撤单量,买入撤单额,卖出撤单笔数,卖出撤单量,卖出撤单额,买入总笔数,卖出总笔数,买入成交最大等待时间,卖出成交最大等待时间,买方委托价位数,卖方委托价位数\
,卖盘价位数,申卖价1,申卖量1,卖出总委托笔数1,申卖价2,申卖量2,卖出总委托笔数2,卖一揭示委托笔数,卖1档队列\
,买盘价位数,申买价1,申买量1,买入总委托笔数1, 申买价2, 申买量2, 买入总委托笔数2,买一档揭示委托笔数,买1档队列,";
}

void GtaQh2csv::writeLine(const void *gtaStruct)/*override*/ {
	const SSEL2_Quotation *q = (const SSEL2_Quotation *)gtaStruct;
	m_ofs << q->LocalTimeStamp << ','		//< 数据接收时间HHMMSSMMM
		<< q->QuotationFlag << ','			//< 1=上海南汇机房行情源；2=深圳福永机房行情源
		<< q->PacketTimeStamp << ','		//< 数据包头时间YYYYMMDDHHMMSSMMM
		<< q->Time << ','					//< 数据生成时间, 最新订单时间（毫秒）;143025001 表示 14:30:25.001
		<< q->Symbol << ','					//< 证券代码, 
		<< q->PreClosePrice << ','			//< 昨收价, 
		<< q->OpenPrice << ','				//< 开盘价, 
		<< q->HighPrice << ','				//< 最高价, 
		<< q->LowPrice << ','				//< 最低价, 
		<< q->LastPrice << ','				//< 现价, 
		<< q->ClosePrice << ','				//< 今收盘价, 
		<< q->TradeStatus[6] << ','			//< 当前品种交易状态,
		<< q->SecurityPhaseTag << ','		//< 当前品种状态
		<< q->TotalNo << ','				//< 成交笔数, 
		<< q->TotalVolume << ','			//< 成交总量, 股票：股;权证：份;债券：手
		<< q->TotalAmount << ','			//< 成交总额, （元）
		<< q->TotalBuyOrderVolume << ','	//< 委托买入总量, 股票：股;权证：份;债券：手
		<< q->WtAvgBuyPrice << ','			//< 加权平均委买价, （元）
		<< q->BondWtAvgBuyPrice << ','		//< 债券加权平均委买价, （元）
		<< q->TotalSellOrderVolume << ','	//< 委托卖出总量, 
		<< q->WtAvgSellPrice << ','			//< 加权平均委卖价, （元）
		<< q->BondWtAvgSellPrice << ','		//< 债券加权平均委卖价, 
		<< q->IOPV << ','					//< ETF 净值估值, 
		<< q->ETFBuyNo << ','				//< ETF 申购笔数, 
		<< q->ETFBuyVolume << ','			//< ETF 申购量, 
		<< q->ETFBuyAmount << ','			//< ETF 申购额, 
		<< q->ETFSellNo << ','				//< ETF 赎回笔数, 
		<< q->ETFSellVolume << ','			//< ETF 赎回量, 
		<< q->ETFSellAmount << ','			//< ETF 赎回额, 
		<< q->YTM << ','					//< 债券到期收益率, 
		<< q->TotalWarrantExecVol << ','	//< 权证执行的总数量, 
		<< q->WarrantDownLimit << ','		//< 权证跌停价格, （元）
		<< q->WarrantUpLimit << ','			//< 权证涨停价格, （元）
		<< q->WithdrawBuyNo << ','			//< 买入撤单笔数, 
		<< q->WithdrawBuyVolume << ','		//< 买入撤单量, 
		<< q->WithdrawBuyAmount << ','		//< 买入撤单额, 
		<< q->WithdrawSellNo << ','			//< 卖出撤单笔数, 
		<< q->WithdrawSellVolume << ','		//< 卖出撤单量, 
		<< q->WithdrawSellAmount << ','		//< 卖出撤单额, 
		<< q->TotalBuyNo << ','				//< 买入总笔数, 
		<< q->TotalSellNo << ','			//< 卖出总笔数, 
		<< q->MaxBuyDuration << ','			//< 买入成交最大等待时间, 
		<< q->MaxSellDuration << ','		//< 卖出成交最大等待时间, 
		<< q->BuyOrderNo << ','				//< 买方委托价位数, 
		<< q->SellOrderNo << ','			//< 卖方委托价位数, 
		<< q->SellLevelNo << ','			//< 卖盘价位数量, 10档行情，不足时补空
		<< q->SellPrice01 << ','			//< 申卖价, 
		<< q->SellVolume01 << ','			//< 申卖量, 
		<< q->TotalSellOrderNo01 << ','		//< 卖出总委托笔数, 
		<< q->SellPrice02 << ','			//< 申卖价, 
		<< q->SellVolume02 << ','			//< 申卖量, 
		<< q->TotalSellOrderNo02 << ','		//< 卖出总委托笔数,
		<< q->SellLevelQueueNo01 << ',';	//< 卖一档揭示委托笔数, 为 0 表示不揭示
	unsigned lq;
	//< 卖1档队列, 高频数据保存，先存订单数量,只有一档有此数据，50档，不足时补空
	for (unsigned i = 0; i < ORDER_LEVEL_FIFTY; ++i) {
		lq = (q->SellLevelQueue)[i];
		if ( lq > 0)
			m_ofs << lq << ';';
		else
			break;
	}
	m_ofs << ',' <<q->BuyLevelNo << ','		//< 买盘价位数量, 10档行情，不足时补空
		<< q->BuyPrice01 << ','				//< 申买价, 
		<< q->BuyVolume01 << ','			//< 申买量, 股票：股;权证：份;债券：手
		<< q->TotalBuyOrderNo01 << ','		//< 买入总委托笔数, 
		<< q->BuyPrice02 << ','				//< 申买价, 
		<< q->BuyVolume02 << ','			//< 申买量, 
		<< q->TotalBuyOrderNo02 << ','		//< 买入总委托笔数,
		<< q->BuyLevelQueueNo01 << ',';		//< 买一档揭示委托笔数, 为 0 表示不揭示
	for (unsigned i = 0; i < ORDER_LEVEL_FIFTY; ++i) {
		lq = (q->BuyLevelQueue)[i];
		if (lq > 0)
			m_ofs << lq << ';';
		else
			break;
	}
	m_ofs << ',' << endl;
}

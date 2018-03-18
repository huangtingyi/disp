#include "QTSStruct.h"
#include "GtaQz2csv.h"

GtaQz2csv::GtaQz2csv(const string &c)
	:Gta2csv(c) {}

void GtaQz2csv::getFileName(string &strFileName)/*override*/ {
	strFileName = "Quotation.csv";
}

void GtaQz2csv::getHeaders(string &strHeaders)/*override*/ {
	strHeaders = "接收,行情源,生成,证券,代码源,昨收,开盘,现价,最高,最低,涨停,跌停,升跌1,升跌2,成交笔数,成交总量,今收,交易状态,市盈率1,基金净值,市盈率2,IOPV,权证溢价率\
,委卖总量,加权委卖价,申卖档位数,申卖价1,申卖量1,卖总委托笔数1,申卖价2,申卖量2,卖总委托笔数2,卖一揭示委托笔数,卖1档队列\
,委买总量,加权委买价,申买档位数,申买价1,申买量1,买总委托笔数1,申买价2,申买量2,买总委托笔数2,买一揭示委托笔数,买1档队列\
,实时加权平均利率,加权平均利率涨跌BP,昨收盘加权平均利率,";
}

void GtaQz2csv::writeLine(const void *gtaStruct)/*override*/ {
	const SZSEL2_Quotation *q = (const SZSEL2_Quotation *)gtaStruct;
	m_ofs << q->LocalTimeStamp << ','		//< 数据接收时间HHMMSSMMM
		<< q->QuotationFlag << ','			//< 1=上海南汇机房行情源；2=深圳福永机房行情源
		<< q->Time << ','					//< 数据生成时间, 最新订单时间（毫秒）;143025001 表示 14:30:25.001
		<< q->Symbol << ','					//< 证券代码,
		<< q->SymbolSource << ','			//< 证券代码源
		<< q->PreClosePrice << ','			//< 昨收价, 
		<< q->OpenPrice << ','				//< 开盘价,
		<< q->LastPrice << ','				//< 现价,
		<< q->HighPrice << ','				//< 最高价, 
		<< q->LowPrice << ','				//< 最低价, 
		<< q->PriceUpLimit << ','			//< 涨停价,6位小数
		<< q->PriceDownLimit << ','			//< 跌停价,6位小数
		<< q->PriceUpdown1 << ','			//< 升跌一,6位小数
		<< q->PriceUpdown2 << ','			//< 升跌二,6位小数
		<< q->TotalNo << ','				//< 成交笔数, 
		<< q->TotalVolume << ','			//< 成交总量, 股票：股;权证：份;债券：手
		<< q->TotalAmount << ','			//< 成交总额, （元）
		<< q->ClosePrice << ','				//< 今收盘价, 
		<< q->SecurityPhaseTag[6] << ','	//< 当前品种交易状态,
		<< q->PERatio1 << ','				//< 市盈率 1,6位小数
		<< q->NAV << ','					//< 基金 T-1 日净值,6位小数
		<< q->PERatio2 << ','				//< 市盈率 2,6位小数
		<< q->IOPV << ','					//< ETF 净值估值,
		<< q->PremiumRate					//< 权证溢价率,6位小数
		<< q->TotalSellOrderVolume << ','	//< 委托卖出总量, 
		<< q->WtAvgSellPrice << ','			//< 加权平均委卖价, （元）
		<< q->SellLevelNo << ','			//< 申卖档位数（价格由低至高）
		<< q->SellPrice01 << ','			//< 申卖价, 
		<< q->SellVolume01 << ','			//< 申卖量, 
		<< q->TotalSellOrderNo01 << ','		//< 卖出总委托笔数, 
		<< q->SellPrice02 << ','			//< 申卖价, 
		<< q->SellVolume02 << ','			//< 申卖量, 
		<< q->TotalSellOrderNo02 << ','		//< 卖出总委托笔数,
		<< q->SellLevelQueueNo01 << ',';	//< 卖一档揭示委托笔数, 为 0 表示不揭示
	double lq;
	//< 卖1档队列, 高频数据保存，先存订单数量,只有一档有此数据，50档，不足时补空
	for (unsigned i = 0; i < ORDER_LEVEL_FIFTY; ++i) {
		lq = (q->SellLevelQueue)[i];
		if (lq > 0)
			m_ofs << lq << ';';
		else
			break;
	}
	m_ofs << ',' << q->TotalBuyOrderVolume << ','//< 委托买入总量, 股票：股;权证：份;债券：手
		<< q->WtAvgBuyPrice << ','			//< 加权平均委买价, （元）
		<< q->BuyLevelNo << ','				//< 申买档位数（价格由高至低）
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
	m_ofs << ',' << q->WtAvgRate << ','			//< 实时加权平均利率    质押式回购产品实时行情增加三个字段
		<< q->WtAvgRateUpdown << ','		//< 加权平均利率涨跌BP
		<< q->PreWtAvgRate << ',';		//< 昨收盘加权平均利率
	m_ofs << endl;
}

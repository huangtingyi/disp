#include "QTSStruct.h"
#include "GtaTh2csv.h"

GtaTh2csv::GtaTh2csv(const string &c)
	:Gta2csv(c) {}

void GtaTh2csv::writeLine(const void *gtaStruct)/*override*/ {
	const SSEL2_Transaction *t = (const SSEL2_Transaction *)gtaStruct;
	m_ofs << t->LocalTimeStamp << ','		//< 数据接收时间HHMMSSMMM
		<< t->QuotationFlag << ','			//< 1=上海南汇机房行情源；2=深圳福永机房行情源
		<< t->PacketTimeStamp << ','		//< 数据包头时间YYYYMMDDHHMMSSMMM
		<< t->TradeTime << ','				//< 成交时间(毫秒), 14302506 表示14:30:25.06
		<< t->RecID << ','					//< 业务索引, 从 1 开始，按 TradeChannel连续
		<< t->TradeChannel << ','			//< 成交通道, 
		<< t->Symbol << ','					//< 证券代码, 
		<< t->TradePrice << ','				//< 成交价格, 
		<< t->TradeVolume << ','			//< 成交数量, 股票：股;权证：份;债券：张
		<< t->TradeAmount << ','			//< 成交金额, 
		<< t->BuyRecID << ','				//< 买方订单号, 
		<< t->SellRecID << ','				//< 卖方订单号,
		<< t->BuySellFlag << ',' << endl;	//< 内外盘标志, B – 外盘,主动买;S – 内盘,主动卖;N – 未知
}

void GtaTh2csv::getFileName(string &strFileName)/*override*/ {
	strFileName = "逐笔成交.csv";
}

void GtaTh2csv::getHeaders(string &strHeaders)/*override*/ {
	strHeaders = "接收,行情源,包头,时间,索引,成交通道,证券,价格,数量,成交金额,买订单号,卖订单号,内外盘,";
}

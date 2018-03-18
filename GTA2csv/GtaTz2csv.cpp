#include "QTSStruct.h"
#include "GtaTz2csv.h"

GtaTz2csv::GtaTz2csv(const string &c)
	:Gta2csv(c) {}

void GtaTz2csv::writeLine(const void *gtaStruct) {
	const SZSEL2_Transaction *t = (const SZSEL2_Transaction *)gtaStruct;
	m_ofs << t->LocalTimeStamp << ','		//< 数据接收时间HHMMSSMMM
		<< t->QuotationFlag << ','			//< 1=上海南汇机房行情源；2=深圳福永机房行情源
		<< t->SetID << ','					//< 证券集代号
		<< t->RecID << ','					//< 消息记录号 从 1 开始计数，同一频道连续
		<< t->BuyOrderID << ','				//< 买方委托索引
		<< t->SellOrderID << ','			//< 卖方委托索引
		<< t->Symbol << ','					//< 证券代码
		<< t->SymbolSource << ','			//< 证券代码源
		<< t->TradeTime << ','				//< 成交时间YYYYMMDDHHMMSSMMM
		<< t->TradePrice << ','				//< 成交价格,4位小数
		<< t->TradeVolume << ','			//< 成交数量,2位小数
		<< t->TradeType << ',' << endl;		//< 成交类别：4=撤销，主动或自动撤单执行报告；F=成交，成交执行报告
}

void GtaTz2csv::getFileName(string &strFileName) {
	strFileName = "逐笔成交.csv";
}

void GtaTz2csv::getHeaders(string &strHeaders) {
	strHeaders = "接收,行情源,证券集,记录号,买委托,卖委托,证券,代码源,时间,价格,数量,成交类别,";
}

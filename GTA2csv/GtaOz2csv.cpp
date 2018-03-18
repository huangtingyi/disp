#include "QTSStruct.h"
#include "GtaOz2csv.h"

GtaOz2csv::GtaOz2csv(const string &c)
	:Gta2csv(c) {}

void GtaOz2csv::getFileName(string &strFileName) {
	strFileName = "逐笔委托.csv";
}

void GtaOz2csv::getHeaders(string &strHeaders) {
	strHeaders = "接收,行情源,频道,记录,证券,代码源,时间,价格,数量,买卖,订单类别,";
}

void GtaOz2csv::writeLine(const void *gtaStruct) {
	const SZSEL2_Order *o = (const SZSEL2_Order *)gtaStruct;
	m_ofs << o->LocalTimeStamp << ','	//< 数据接收时间HHMMSSMMM
		<< o->QuotationFlag << ','		//< 1=上海南汇机房行情源；2=深圳福永机房行情源
		<< o->SetID << ','				//< 频道代码
		<< o->RecID << ','				//< 消息记录号:从 1 开始计数，同一频道连续
		<< o->Symbol << ','				//< 证券代码
		<< o->SymbolSource << ','		//< 证券代码源
		<< o->Time << ','				//< 委托时间YYYYMMDDHHMMSSMMM
		<< o->OrderPrice << ','			//< 委托价格,4位小数
		<< o->OrderVolume << ','		//< 委托数量,2位小数
		<< o->OrderCode << ','			//< 买卖方向：1=买 2=卖 G=借入 F=出借
		<< o->OrderType << ',' << endl;	//< 订单类别：1=市价 2=限价 U=本方最优
}

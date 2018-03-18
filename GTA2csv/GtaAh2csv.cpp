#include "QTSStruct.h"
#include "GtaAh2csv.h"

GtaAh2csv::GtaAh2csv(const string &c)
	:Gta2csv(c) {}

void GtaAh2csv::getFileName(string &strFileName) {
	strFileName = "Auction.csv";
}

void GtaAh2csv::getHeaders(string &strHeaders) {
	strHeaders = "采集,行情源,包头,生成,证券,虚参考,虚匹配,虚未匹配,买卖,";
}

void GtaAh2csv::writeLine(const void *gtaStruct) {
	const SSEL2_Auction *a = (const SSEL2_Auction *)gtaStruct;
	m_ofs << a->LocalTimeStamp << ','		//< 数据接收时间HHMMSSMMM
		<< a->QuotationFlag << ','			//< 1=上海南汇机房行情源；2=深圳福永机房行情源
		<< a->PacketTimeStamp << ','		//< 数据包头时间YYYYMMDDHHMMSSMMM
		<< a->Time << ','					//< 数据生成时间, 最新订单时间（毫秒）;143025001 表示 14:30:25.001
		<< a->Symbol << ','					//< 证券代码, 
		<< a->OpenPrice << ','				//< 虚拟开盘价, 
		<< a->AuctionVolume << ','			//< 虚拟匹配量， 
		<< a->LeaveVolume << ','			//< 虚拟未匹配量, 
		<< a->Side << ',' << endl;			//< 买卖方向
}

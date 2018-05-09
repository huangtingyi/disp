#include "tdf_supp.h"


void TDF_MARKET_DATA2MktData(MktData &output, const TDF_MARKET_DATA &src)
{
	output.set_szcode(strtoul(src.szCode,nullptr,10));	//600001.SH
	output.set_ntime(src.nTime);			//时间(HHMMSSmmm)
	output.set_nstatus(src.nStatus);		//状态
	output.set_npreclose(uint32_t(src.nPreClose));	//前收盘价
	output.set_nopen(uint32_t(src.nOpen));			//开盘价
	output.set_nhigh(uint32_t(src.nHigh));			//最高价
	output.set_nlow(uint32_t(src.nLow));				//最低价
	output.set_nmatch(uint32_t(src.nMatch));			//最新价
	for (unsigned char i = 0; i < 10; ++i) {
		output.add_naskprice(uint32_t(src.nAskPrice[i]));//申卖价
		output.add_naskvol(src.nAskVol[i]);	//申卖量
		output.add_nbidprice(uint32_t(src.nBidPrice[i]));//申买价
		output.add_nbidvol(src.nBidVol[i]);	//申买量
	}
	output.set_nnumtrades(src.nNumTrades);	//成交笔数
	output.set_ivolume(src.iVolume);		//成交总量
	output.set_iturnover(src.iTurnover);	//成交总金额
	output.set_ntotalbidvol(src.nTotalBidVol);//委托买入总量
	output.set_ntotalaskvol(src.nTotalAskVol);//委托卖出总量
	output.set_nweightedavgbidprice(uint32_t(src.nWeightedAvgBidPrice));//加权平均委买价格
	output.set_nweightedavgaskprice(uint32_t(src.nWeightedAvgAskPrice));//加权平均委卖价格
	output.set_niopv(src.nIOPV);		//IOPV净值估值
	output.set_nyieldtomaturity(src.nYieldToMaturity);//到期收益率
	output.set_nhighlimited(uint32_t(src.nHighLimited));//涨停价
	output.set_nlowlimited(uint32_t(src.nLowLimited));//跌停价
	output.set_nsyl1(src.nSyl1);			//市盈率1
	output.set_nsyl2(src.nSyl2);			//市盈率2
	output.set_nsd2(src.nSD2);				//升跌2（对比上一笔）
}

void TDF_TRANSACTION2Transaction(Transaction &output, const TDF_TRANSACTION &src)
{
	output.set_szcode(strtoul(src.szCode, nullptr, 10));	//600001.SH
	output.set_ntime(src.nTime);			//成交时间(HHMMSSmmm)
	output.set_nindex(src.nIndex);			//成交编号
	output.set_nprice(uint32_t(src.nPrice));//成交价格
	output.set_nvolume(src.nVolume);		//成交数量
	output.set_nturnover(src.nTurnover);	//成交金额
	output.set_nbsflag(src.nBSFlag);		//买卖方向(买：'B', 卖：'S', 不明：' ')
	output.set_chorderkind(src.chOrderKind);//成交类别
	output.set_chfunctioncode(src.chFunctionCode);//成交代码
	output.set_naskorder(src.nAskOrder);	//叫卖方委托序号
	output.set_nbidorder(src.nBidOrder);
}

void TDF_ORDER2Order(Order &output, const TDF_ORDER&src)
{
	output.set_szcode(strtoul(src.szCode, nullptr, 10));	//600001.SH
	output.set_ntime(src.nTime);			//时间(HHMMSSmmm)
	output.set_norder(src.nOrder);		//委托号
	output.set_nprice(uint32_t(src.nPrice));//委托价格
	output.set_nvolume(src.nVolume);		//委托数量
	output.set_chorderkind(src.chOrderKind);//委托类别
	output.set_chfunctioncode(src.chFunctionCode);//委托代码('B','S','C')
	output.set_nbroker(src.nBroker);		//经纪商编码
	output.set_chstatus(src.chStatus);		//委托状态
	output.set_chflag(src.chFlag);
}

void TDF_ORDER_QUEUE2Order_queue(Order_queue &output, const TDF_ORDER_QUEUE&src)
{
	output.set_szcode(strtoul(src.szCode, nullptr, 10));	//600001.SH
	output.set_ntime(src.nTime);			//时间(HHMMSSmmm)
	output.set_nside(src.nSide=='B'||src.nSide=='b');//买卖方向('B':Bid 'A':Ask)
	output.set_nprice(uint32_t(src.nPrice));//委托价格
	output.set_norders(src.nOrders);		//订单数量
	output.set_nabitems(src.nABItems);		//明细个数
	for (int i = 0; i < src.nABItems; ++i) {
		output.add_nabvolume(src.nABVolume[i]);
	}
}

void D31_ITEM2D31Item(D31Item &o, const struct D31ItemStruct &d)
{
	int i;

        o.set_nstockcode(d.nStockCode);
        o.set_ntradetime(d.nTradeTime);
        
        for(i=0;i<10;i++) o.add_afzbbidamount(d.afZbBidAmount[i]);
	for(i=0;i<10;i++) o.add_afzbbidvolume(d.afZbBidVolume[i]);
	for(i=0;i<10;i++) o.add_anzbbidordernum(d.anZbBidOrderNum[i]);
	for(i=0;i<10;i++) o.add_afzbaskamount(d.afZbAskAmount[i]);
	for(i=0;i<10;i++) o.add_afzbaskvolume(d.afZbAskVolume[i]);
	for(i=0;i<10;i++) o.add_anzbaskordernum(d.anZbAskOrderNum[i]);
	for(i=0;i<10;i++) o.add_afzdbidamount(d.afZdBidAmount[i]);
	for(i=0;i<10;i++) o.add_afzdbidvolume(d.afZdBidVolume[i]);
	for(i=0;i<10;i++) o.add_anzdbidordernum(d.anZdBidOrderNum[i]);
	for(i=0;i<10;i++) o.add_afzdaskamount(d.afZdAskAmount[i]);
	for(i=0;i<10;i++) o.add_afzdaskvolume(d.afZdAskVolume[i]);
	for(i=0;i<10;i++) o.add_anzdaskordernum(d.anZdAskOrderNum[i]);

        o.set_ftenbidvolume(	    d.fTenBidVolume);
	o.set_ftenaskvolume(        d.fTenAskVolume);
	o.set_ftenbidamnt(          d.fTenBidAmnt);
	o.set_ftenaskamnt(          d.fTenAskAmnt);
	o.set_ftotalbidvolume(      d.fTotalBidVolume);
	o.set_ftotalaskvolume(      d.fTotalAskVolume);
	o.set_ftotalbidamnt(        d.fTotalBidAmnt);
	o.set_ftotalaskamnt(        d.fTotalAskAmnt);
	o.set_fwtavgbidprice(       d.fWtAvgBidPrice);
	o.set_fwtavgaskprice(       d.fWtAvgAskPrice);
	o.set_flastclose(           d.fLastClose);
	o.set_fcurprice(            d.fCurPrice);
	o.set_favgtotalbidamnt(     d.fAvgTotalBidAmnt);
	o.set_favgtotalaskamnt(     d.fAvgTotalAskAmnt);
	o.set_fbidamount20(         d.fBidAmount20);
	o.set_faskamount20(         d.fAskAmount20);
	o.set_fbidamount50(         d.fBidAmount50);
	o.set_faskamount50(         d.fAskAmount50);
	o.set_fbidamount100(        d.fBidAmount100);
	o.set_faskamount100(        d.fAskAmount100);
	
	for(i=0;i<27;i++) o.add_afreserve(d.afReserve[i]);
}

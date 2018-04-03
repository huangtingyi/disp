#pragma once
#include <cstring>
#include <cstdint>
#include "../Include/QTSStruct.h"
#include "../IncludeTDF/TDFAPIStruct.h"

#ifndef MAX_STOCK_CODE
#define MAX_STOCK_CODE	1000000
#endif

struct LimitUpDownStruct{
	double	WarrantDownLimit;       ///< 股票跌停价格, （元）
	double	WarrantUpLimit;         ///< 股票涨停价格, （元）
};

extern struct LimitUpDownStruct LIMIT[MAX_STOCK_CODE];

void InitLimitArray();
int64_t yuan2percentFen(const double yuan);
char GTA2TDF_MARKET_STATUS(const char *TradeStatus, const char *SecurityPhaseTag);
void GTA2TDF_SSEL2(const SSEL2_Quotation &src, TDF_MARKET_DATA &m, TDF_ORDER_QUEUE &qB, TDF_ORDER_QUEUE &qS);
void GTA2TDF_SSEL2_T(const SSEL2_Transaction &src, TDF_TRANSACTION &t);
void GTA2TDF_SZSEL2(const SZSEL2_Quotation &src, TDF_MARKET_DATA &m, TDF_ORDER_QUEUE &qB, TDF_ORDER_QUEUE &qS);
void  GTA2TDF_SZSEL2_T(const SZSEL2_Transaction &src, TDF_TRANSACTION &t);
void GTA2TDF_SZSEL2_O(const SZSEL2_Order &src, TDF_ORDER &o);
void  GTA2TDF_SSEL2_AM(const SSEL2_Auction &src, TDF_MARKET_DATA &m);

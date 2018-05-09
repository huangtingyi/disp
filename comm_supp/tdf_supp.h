#ifndef __TDF_SUPP_H__
#define __TDF_SUPP_H__

#include "mktdata.pb.h"
#include "d31data.pb.h"
#include "TDFAPIStruct.h"

#include "d31_item.h"

void TDF_MARKET_DATA2MktData(MktData &output, const TDF_MARKET_DATA &src);
void TDF_TRANSACTION2Transaction(Transaction &output, const TDF_TRANSACTION &src);
void TDF_ORDER2Order(Order &output, const TDF_ORDER&src);
void TDF_ORDER_QUEUE2Order_queue(Order_queue &output, const TDF_ORDER_QUEUE&src);

void D31_ITEM2D31Item(D31Item &o, const struct D31ItemStruct &d);


#endif

#pragma once
#include "VectorStockCode.h"

class VectorStockCodeSZ : public VectorStockCode {
protected:
	inline bool code_filter(const char *stockCode) override {
		if (stockCode[8] == 'Z'
			&& (
					(stockCode[0] == '3' && stockCode[1] == '0')
					|| (stockCode[0] == '0' && stockCode[1] == '0')
					|| (stockCode[0] == '1' && stockCode[1] == '2')
				)
			)
		{
			return true;
		}
		return false;
	};
};

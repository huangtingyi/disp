#pragma once
#include "VectorStockCode.h"

class VectorStockCodeSH : public VectorStockCode {
protected:
	inline bool code_filter(const char *stockCode) override {
		if (stockCode[8] == 'S'
			&&	(
					(stockCode[0] == '6' && stockCode[1] == '0')
					|| (stockCode[0] == '1' && stockCode[1] == '1')
				)
		)
		{
			return true;
		}			
		return false;
	};
};

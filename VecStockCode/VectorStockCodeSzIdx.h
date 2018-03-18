#pragma once
#include "VectorStockCode.h"

class VectorStockCodeSzIdx : public VectorStockCode {
protected:
	bool code_filter(const char *stockCode) override;
};
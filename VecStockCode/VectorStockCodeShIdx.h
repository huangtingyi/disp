#pragma once
#include "VectorStockCode.h"

class VectorStockCodeShIdx : public VectorStockCode {
protected:
	bool code_filter(const char *stockCode) override;
};
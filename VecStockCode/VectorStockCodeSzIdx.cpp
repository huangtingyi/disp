#include "VectorStockCodeSzIdx.h"
#if 0
399001	深证成指
399002	深成指R
399003	成份B指
399005	中小板指
399006	创业板指
399300	沪深300
#endif
bool VectorStockCodeSzIdx::code_filter(const char *stockCode) {
	if (stockCode[8] == 'Z') {
		string strCode = stockCode;
		strCode = strCode.substr(0, 6);
		int code = stoi(strCode);
		switch (code) {
		case 399001:
		case 399002:
		case 399003:
		case 399005:
		case 399006:
		case 399300:
			return true;
		}
	}
	return false;
}

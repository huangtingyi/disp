#include "VectorStockCodeShIdx.h"
#if 0
990009	上证380		000009
990132	上证100		000132
990133	上证150		000133
990851	百发100		000851
990852	中证1000	000852
990903	中证100		000903
990904	中证200		000904
990905	中证500		000905
990906	中证800		000906
990907	中证700		000907
999987	上证50		000016
999991	上证180		000010
999997	B股指数		000003
999998	A股指数		000002
999999	上证指数	000001
000300	沪深300		000300
#endif
bool VectorStockCodeShIdx::code_filter(const char *stockCode) {
	if (stockCode[8] == 'S') {
		string strCode = stockCode;
		strCode = strCode.substr(0, 6);
		int code = stoi(strCode);
		switch (code) {
			case 9:
			case 132:
			case 133:
			case 851:
			case 852:
			case 903:
			case 904:
			case 905:
			case 906:
			case 907:
			case 16:
			case 10:
			case 3:
			case 2:
			case 1:
			case 300:
				return true;
		}
	}
	return false;
}

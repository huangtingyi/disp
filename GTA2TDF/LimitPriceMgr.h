#pragma once
#include <cstdint>
#include <map>
#include <string>
#include "util.h"
using namespace std;

struct LimitPrices{
	int64_t high;
	int64_t low;
};

class LimitPriceMgr {
public:
	inline void update(const string &code, const double high, const double low) {
		LimitPrices l = {
			yuan2percentFen(high),
			yuan2percentFen(low)
		};
		auto p = m_map.insert({ code, l });
		if (!(p.second)) {
			p.first->second = l;
		}
	}
	inline bool getLimitPrices(int64_t &high, int64_t &low, const string &code) {
		const auto it = m_map.find(code);
		if (it != m_map.cend()) {
			high = it->second.high;
			low = it->second.low;
			return true;
		}
		return false;
	}
protected:
	map<string, LimitPrices> m_map;
};

extern LimitPriceMgr g_LimitPriceMgr;

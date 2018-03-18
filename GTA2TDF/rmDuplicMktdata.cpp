#include "rmDuplicMktdata.h"
#include "../IncludeTDF/TDFAPIStruct.h"

#define IN_AUCTION_TIME(t) (t>=91500000 && t<92600000)
#define IN_TRADE_TIME(t) ((t>=93000000 && t<113100000) || (t>=130000000 && t<150200000))

bool RmDuplicMarketdata::rmDuplic(shared_ptr<TDF_MARKET_DATA> m) {
	auto findCode = m_map.find(m->szWindCode);
	if (findCode != m_map.end()) {
		shared_ptr<TDF_MARKET_DATA> &old = findCode->second;		
		if (m->iVolume > old->iVolume) {
			old = m;
			return false;
		}
		if (m->iVolume == old->iVolume) {
			int thisTime = m->nTime;
			if ( (thisTime > old->nTime && IN_TRADE_TIME(thisTime))
				|| (IN_AUCTION_TIME(thisTime) && memcmp(&m, &old, sizeof(TDF_MARKET_DATA))) ) {
				old = m;
				return false;
			}
		}
		return true;
	}
	m_map.insert({ m->szWindCode, m });
	return false;
}

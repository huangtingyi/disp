#include "../IncludeTDF/TDFAPIStruct.h"
#include "rmDuplicTransaction.h"

bool RmDuplicTransaction::rmDuplic(shared_ptr<TDF_TRANSACTION> &t) {
	auto findCode = m_map.find(t->szWindCode);
	if (findCode != m_map.end()) {
		if (t->nIndex > findCode->second->nIndex) {
			findCode->second = t;
			return false;
		}
		return true;
	}
	m_map.insert({ t->szWindCode, t });
	return false;
}

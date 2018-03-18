#pragma once
#include <memory>
#include <map>
using namespace std;

struct TDF_TRANSACTION;

class RmDuplicTransaction {
public:
	bool rmDuplic(shared_ptr<TDF_TRANSACTION> &t);
protected:
	map<const string, shared_ptr<TDF_TRANSACTION> > m_map;
};

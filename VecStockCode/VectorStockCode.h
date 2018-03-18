#pragma once
#include <vector>
#include <string>
using namespace std;

class VectorStockCode
{
public:
	inline bool push(const char *stockCode) {
		if (code_filter(stockCode)) {
			m_vec.push_back(stockCode);
			return true;
		}
		return false;
	};
	inline void strForSub(string &output) {
		for (const string &strCode : m_vec) {
			output += strCode.substr(0, 6);
			output += ',';
		}
		//如果不加入size判断，则下一句core出
		if(m_vec.size()>0)
			output.pop_back();
	};
	inline const vector<string> & codes() const {
		return m_vec;
	};
protected:
	vector<string> m_vec;
	virtual bool code_filter(const char *stockCode) = 0;
};

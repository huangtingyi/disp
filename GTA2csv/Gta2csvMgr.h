#pragma once

#include <map>
#include <memory>
#include "QTSStruct.h"
#include "../VecStockCode/VectorStockCodeSH.h"
#include "../VecStockCode/VectorStockCodeSZ.h"
#include "Gta2csv.h"
#include "csvWriter.h"
using namespace std;

//struct CsvHwriter;
//struct CsvZwriter;

class Gta2csvMgr {
public:
	void init(const string &dir, const vector<string> *vSh, const vector<string> *vSz);
	void writeCsv(const SSEL2_Quotation &input) const;
	void writeCsv(const SSEL2_Transaction &input) const;
	void writeCsv(const SSEL2_Auction &input) const;
	void writeCsv(const SZSEL2_Quotation &input) const;
	void writeCsv(const SZSEL2_Transaction &input) const;
	void writeCsv(const SZSEL2_Order &input) const;
protected:
	map<string, CsvHwriter> m_csvHwriters;
	map<string, CsvZwriter> m_csvZwriters;
};

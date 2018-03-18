#pragma once
#include "Gta2csv.h"
using namespace std;

class GtaTz2csv: public Gta2csv {
public:
	GtaTz2csv(const string &c);
	void writeLine(const void *gtaStruct) override;
protected:
	void getFileName(string &strFileName) override;
	void getHeaders(string &strHeaders) override;
};

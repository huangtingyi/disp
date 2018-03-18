#pragma once
#include "Gta2csv.h"

class GtaAh2csv : public Gta2csv {
public:
	GtaAh2csv(const string &c);
	void writeLine(const void *gtaStruct) override;
protected:
	void getFileName(string &strFileName) override;
	void getHeaders(string &strHeaders) override;
};

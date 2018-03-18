#pragma once
#include <string>
#include <fstream>
using namespace std;

class Gta2csv {
public:
	Gta2csv(const string &c);
	inline static void setDir(const string &d) {
		dir = d;
	};
	void createFile();
	virtual void writeLine(const void *gtaStruct) = 0;
protected:
	static string dir;
	const string m_code;
	ofstream m_ofs;
	virtual void getFileName(string &strFileName) = 0;
	virtual void getHeaders(string &strHeaders) = 0;
};

#include <boost/filesystem.hpp>
#include "Gta2csv.h"

#include "../public/getToday.h"

string Gta2csv::dir;

Gta2csv::Gta2csv(const string &c)
	: m_code(c) {}

void Gta2csv::createFile() {
	string strTmp;
	getFileName(strTmp);
	//string dirCode = GTA2csv::dir + '\\' + getToday() + '\\' + m_code + '\\';
	//_mkdir(dirCode.c_str());
	boost::filesystem::path p = Gta2csv::dir;

	string dirCode = p.generic_string();
	dirCode = dirCode + '/' + getToday() + '/' + m_code + '/';
	p = dirCode;
	boost::filesystem::create_directories(p);
	
	
	strTmp = dirCode + strTmp;
	bool exists = false;
	if (!boost::filesystem::exists(strTmp)) {
		exists = true;
	}
	m_ofs.open(strTmp, ofstream::app);
	if (!exists) {
		getHeaders(strTmp);
		m_ofs << strTmp << endl;
	}

}

#include <boost/filesystem.hpp>
#include "Gta2csv.h"
#include "wwtiny.h"


string Gta2csv::dir;

Gta2csv::Gta2csv(const string &c)
	: m_code(c) {}

void Gta2csv::createFile() 
{
	char sHostTime[15];

	string strTmp;
	getFileName(strTmp);

	boost::filesystem::path p = Gta2csv::dir;

	GetHostTime(sHostTime);
	
	string todayStr;
	string dirCode = p.generic_string();
	
	todayStr.assign(sHostTime,8);

	dirCode = dirCode + '/' + todayStr + '/' + m_code + '/';
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

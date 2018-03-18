#include <sys/stat.h>

#include "Gta2csvMgr.h"
#include "GtaAh2csv.h"

void Gta2csvMgr::init(const string &dir, const vector<string> *vSh, const vector<string> *vSz) {

	mkdir(dir.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	Gta2csv::setDir(dir);
	if (vSh) {
		for (const auto &eachStrCode : *vSh) {
			shared_ptr<GtaQh2csv> pQ = make_shared<GtaQh2csv>(eachStrCode);
			pQ->createFile();
			shared_ptr<GtaTh2csv> pT = make_shared<GtaTh2csv>(eachStrCode);
			pT->createFile();
			shared_ptr<GtaAh2csv> pA = make_shared<GtaAh2csv>(eachStrCode);
			pA->createFile();
			CsvHwriter c{ pQ, pT, pA };
			m_csvHwriters.insert({eachStrCode, c});
		}
	}
	if (vSz) {
		for (const auto &eachStrCode : *vSz) {
			shared_ptr<GtaQz2csv> pQ = make_shared<GtaQz2csv>(eachStrCode);
			pQ->createFile();
			shared_ptr<GtaTz2csv> pT = make_shared<GtaTz2csv>(eachStrCode);
			pT->createFile();
			shared_ptr<GtaOz2csv> pO = make_shared<GtaOz2csv>(eachStrCode);
			pO->createFile();
			CsvZwriter c{ pQ, pT, pO };
			m_csvZwriters.insert({ eachStrCode, c });
		}
	}
}

void Gta2csvMgr::writeCsv(const SSEL2_Quotation &input) const {
	const auto it = m_csvHwriters.find(input.Symbol);
	if (it != m_csvHwriters.cend()) {
		it->second.q->writeLine((const void *)&input);
	}
}

void Gta2csvMgr::writeCsv(const SSEL2_Transaction &input) const {
	const auto it = m_csvHwriters.find(input.Symbol);
	if (it != m_csvHwriters.cend()) {
		it->second.t->writeLine((const void *)&input);
	}
}

void Gta2csvMgr::writeCsv(const SSEL2_Auction &input) const {
	const auto it = m_csvHwriters.find(input.Symbol);
	if (it != m_csvHwriters.cend()) {
		it->second.a->writeLine((const void *)&input);
	}
}

void Gta2csvMgr::writeCsv(const SZSEL2_Quotation &input) const {
	const auto it = m_csvZwriters.find(input.Symbol);
	if (it != m_csvZwriters.cend()) {
		it->second.q->writeLine((const void *)&input);
	}
}

void Gta2csvMgr::writeCsv(const SZSEL2_Transaction &input) const {
	const auto it = m_csvZwriters.find(input.Symbol);
	if (it != m_csvZwriters.cend()) {
		it->second.t->writeLine((const void *)&input);
	}
}

void Gta2csvMgr::writeCsv(const SZSEL2_Order &input) const {
	const auto it = m_csvZwriters.find(input.Symbol);
	if (it != m_csvZwriters.cend()) {
		it->second.o->writeLine((const void *)&input);
	}
}

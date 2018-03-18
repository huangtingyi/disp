#pragma once
#include <memory>
#include "GtaQh2csv.h"
#include "GtaTh2csv.h"
#include "GtaQz2csv.h"
#include "GtaTz2csv.h"
#include "GtaOz2csv.h"
using namespace std;

class GtaAh2csv;

struct CsvHwriter {
	shared_ptr<GtaQh2csv> q;
	shared_ptr<GtaTh2csv> t;
	shared_ptr<GtaAh2csv> a;
};

struct CsvZwriter {
	shared_ptr<GtaQz2csv> q;
	shared_ptr<GtaTz2csv> t;
	shared_ptr<GtaOz2csv> o;
};

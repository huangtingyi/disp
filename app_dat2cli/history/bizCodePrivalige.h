#pragma once
#include <set>
#include <string>
#include "mktdata.pb.h"
using namespace std;

struct Privalige
{
	string m_sIp;
	string m_sUser;
	int m_iMqID;
	string password;
	set<BizCode> privl;
};

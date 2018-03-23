#pragma once
#include <ctime>
#include <cstdio>
#include <string>
using namespace std;
extern char g_today[9];
extern uint32_t g_todayYyyymmdd;

inline void getTodayInit() {
	
	struct tm *tm;
	time_t	hosttime;

	time(&hosttime);
	
	tm=(struct tm*)localtime(&hosttime);

	if(tm!=NULL){
		sprintf(g_today, "%04d%02d%02d", tm->tm_year + 1900, tm->tm_mon+1, tm->tm_mday);

		g_todayYyyymmdd = (tm->tm_year + 1900) * 10000 + (tm->tm_mon + 1) * 100 + tm->tm_mday;
	}
}

inline const char *getToday() {
	return g_today;
}

inline uint32_t getTodayYyyymmdd() {
	return g_todayYyyymmdd;
}

uint32_t getMilSec();
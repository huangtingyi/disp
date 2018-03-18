
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "getToday.h"

char g_today[9];
uint32_t g_todayYyyymmdd;

uint32_t getMilSec() {
	struct timeval tv;
	gettimeofday(&tv,NULL);

	return tv.tv_sec*1000 + tv.tv_usec/1000;
}
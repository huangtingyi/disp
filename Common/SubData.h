#pragma once
#include <string>
#include "QTSDataType.h"
using namespace std;

typedef struct _sub_data_
{
	MsgType     msgType;    ///< 消息类型
	long long	cur_time;	///< 接收到数据时间，精确至毫秒，格式为：DDHHMMSSmmm
	std::string	data;		///< 数据内容
}SubData;

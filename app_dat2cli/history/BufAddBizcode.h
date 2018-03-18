#pragma once
#include <arpa/inet.h>
#include <string>
#include <google/protobuf/message.h>
#include "mktdata.pb.h"
using namespace std;
class BufAddBizcode
{
public:
	//将bizCode和pbmsg信息打包成为串 格式：2字节长度+1字节bizCode+pb->SerializeToString结果，长度为 1字节+序列化串
	static void addBizcode(string &output, const google::protobuf::Message &pbmsg, BizCode bizCode);
	//将msgInput的第一个字段取出，作为bizCode后面的串作为pbmsg
	static void getBizcode(BizCode &bizCode, string &pbmsg, const string &msgInput);
};


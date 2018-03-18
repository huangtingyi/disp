#include "BufAddBizcode.h"

//将bizCode和pbmsg信息打包成为串 格式：2字节长度+1字节bizCode+pb->SerializeToString结果，长度为 1字节+序列化串
void BufAddBizcode::addBizcode(string &output, const google::protobuf::Message &pbmsg, BizCode bizCode)
{
	string msg;

	pbmsg.SerializeToString(&msg);

	char strBizCode[2] = { 0 };

	strBizCode[0] = bizCode;
	output = string(strBizCode) + msg;

	uint16_t networkLenBody = uint16_t(htons(output.size()));

	char sLen[3]={0};

	sLen[0] = ((char*)&networkLenBody)[0];
	sLen[1] = ((char*)&networkLenBody)[1];
	output.insert(0,sLen,2);
}
//将msgInput的第一个字段取出，作为bizCode后面的串作为pbmsg
void BufAddBizcode::getBizcode(BizCode &bizCode, string &pbmsg, const string &msgInput)
{
	bizCode = BizCode((unsigned char) (msgInput[0]));
	pbmsg = msgInput.substr(1, string::npos);
}

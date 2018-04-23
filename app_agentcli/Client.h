#pragma once
#include <string>
#include <set>
#include <atomic>
#include <memory>
#include <chrono>
#include <iostream>
#include <thread>
#include <sstream>
#include "TcpClient.h"
#include <google/protobuf/message.h>
#include "marketInterface.h"
#include "getToday.h"
#include "mktdata.pb.h"

using namespace std;

#define	MAX_PATH	(260)

extern void sendMsg2Client(string &msg);

class Client
{
public:
	Client();
	inline void setUser(const string &username)
	{
		m_username = username;
	}
	;
	inline void setPassword(const string &password)
	{
		m_password = password;
	}
	;
	inline void setTcpClient(TcpClient *tcpClient)
	{
		m_tcpClient = tcpClient;
		tcpClient->setClient(this);
	}
	;
	bool connect(string &errmsg)
	{
		return m_tcpClient->connect(errmsg);
	}
	;
	void login();
	void recv(string &msg);
	void subscribeReq(bool marketdata, bool transaction, bool orderqueue, bool order, bool replay = false, uint32_t date = false, uint32_t begin = false, uint32_t end = false);
	void handle_error(const string &errmsg);
	inline void stop()
	{
		if (m_tcpClient)
		{
			m_tcpClient->stop();
		}
	}
	;
	void handle_connect(bool c = true);
	bool *m_connected;
	uint32_t yyyymmddReplay;
	TcpClient *m_tcpClient;
	int32_t getCodes(int32_t *codes);
	void addReduceCode(int8_t addReduce, int32_t *codes, int32_t size);
	void getBizcode(BizCode &bizCode, string &pbmsg, const string &msgInput);
	void addBizcode(string &output, const google::protobuf::Message &pbmsg, BizCode bizCode);
	atomic<uint8_t> m_loginSuccess;
	string m_loginErrMsg;
private:
	string m_username;
	string m_password;
	void async_send(const google::protobuf::Message &msg, BizCode bizCode);
	unsigned m_secHeartbeat;
	void beginHeartbeat();
	set<int32_t> m_codes;
	bool m_codesReady;
};

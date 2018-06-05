#include <atomic>
#include "Client.h"
#include "marketInterface.h"
#include "wwtiny.h"

#include "mktdata.pb.h"
#include "d31data.pb.h"

#include "callsupp.h"

int getTodayYyyymmdd()
{
	char sHostTime[15];
	
	GetHostTime(sHostTime);sHostTime[8]=0;
	
	return atoi(sHostTime);
}
Client::Client() :
	yyyymmddReplay(getTodayYyyymmdd()),
	m_tcpClient(nullptr)
{
	m_loginSuccess = M_SUCCESS;
	m_codesReady = false;
}

void Client::login()
{
	LoginRequest req;
	req.set_name(m_username);
	req.set_password(m_password);
	async_send(req, BizCode::LOGIN_REQ);
}

void Client::async_send(const google::protobuf::Message &msg, BizCode bizCode)
{
	string msgBody;
	addBizcode(msgBody, msg, bizCode);
	m_tcpClient->writeThreadSafe(msgBody);
}

void Client::recv(string &msg)
{
	int iStockCode=0;
	string msgProtobuf; //剥离了bizcode序列化了的pb数据
	BizCode iBizCode;
	getBizcode(iBizCode, msgProtobuf, msg);
	switch (iBizCode)
	{
	case LOGIN_REP:
	{
		Reply rep;
		rep.ParseFromString(msgProtobuf);
		cout << "rep.err=" << rep.err() << endl;
		if (rep.err() == SUCCESS)
		{
			m_loginSuccess = M_SUCCESS;
			cout << "login success" << endl;
			try
			{
				m_secHeartbeat = stoul(rep.desc());
			}
			catch (...)
			{
				m_secHeartbeat = 20;
			}
			beginHeartbeat();
		}
	}
		break;
	case MKT_DATA_LV2:
	{
		MktData 	t;
		t.ParseFromString(msgProtobuf);
		iStockCode=	t.szcode();
		SendMsg2Cli(iStockCode,'M',msgProtobuf);
	}
	break;
	case MKT_DATA_DEAL:
	{
		Transaction 	t;
		t.ParseFromString(msgProtobuf);
		iStockCode=	t.szcode();
		SendMsg2Cli(iStockCode,'T',msgProtobuf);
	}
	break;
	case SZ_ORDER:
	{
		Order 		t;
		t.ParseFromString(msgProtobuf);
		iStockCode=	t.szcode();
		SendMsg2Cli(iStockCode,'O',msgProtobuf);
	}
	break;
	case MKT_DATA_ORDERQUEUE:
	{
		Order_queue	t;
		t.ParseFromString(msgProtobuf);
		iStockCode=	t.szcode();
		SendMsg2Cli(iStockCode,'Q',msgProtobuf);
	}
	break;
	case D31_ITEM:
	{
		D31Item         t;
		t.ParseFromString(msgProtobuf);
		iStockCode=	t.nstockcode();

		//设置callsupp.cpp中定义的全局变量，供D31TradeTimeValid使用
		nD31TradeTime=	t.ntradetime();

		SendMsg2Cli(iStockCode,'D',msgProtobuf);
	}
	break;
	case INDEX_SH:
	case INDEX_SZ:
	{
		//目前系统没这类数据，不实现这部分了
	}
		break;
	case BizCode::CODES_BROADCAST:
	{
		PbCodesBroadcast pbMsg;
		pbMsg.ParseFromString(msgProtobuf);
		const int sz = pbMsg.codes_size();
		const auto codes = pbMsg.codes();
		for (int i = 0; i < sz; ++i)
		{
			m_codes.insert(codes[i]);
		}
		m_codesReady = true;
	}
		break;
	case FOR_TEST:
	{
		time_t timeNow = time(nullptr);
		tm *tmNow = localtime(&timeNow);
		printf("RECV last msg at %02d%02d%02d\n", tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec);
		//g_sendComplete = true;
	}
		break;
	default: break;
	}
}

void Client::subscribeReq(bool marketdata, bool transaction, bool orderqueue, bool order,
	 bool replay, uint32_t timeBegin, uint32_t timeEnd, uint32_t date)
{
	SubscribeRequest req;
	if (marketdata)
		req.add_sub(proto_sub_LV2);
	if (transaction)
		req.add_sub(proto_sub_DEAL);
	if (orderqueue)
		req.add_sub(proto_sub_ORDERQUEUE);
	if (order)
		req.add_sub(proto_sub_SZ_ORDER);
	if (replay)
	{
		req.set_replay(true);
		if (date > 0)
		{
			req.set_date(date);
			if (timeBegin > 0)
				req.set_timebegin(timeBegin);
			else
				req.set_timebegin(91500);
			if (timeEnd > 0)
				req.set_timeend(timeEnd);
			else
				req.set_timeend(153000);
		}
	}

	async_send(req, SUBSCRIBLE);
}

void Client::handle_error(const string &errmsg)
{
	time_t timeNow = time(nullptr);
	tm *tmNow = localtime(&timeNow);

	printf("ERROR  msg at %02d%02d%02d:%s\n",
		tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec,errmsg.c_str());
}

void Client::handle_connect(bool c)
{
	if (c)
	{
		*m_connected = true;
	}
}

void Client::beginHeartbeat()
{
	PbHeartbeat heartbeat;
	heartbeat.set_nothing(false);
	string output;
	addBizcode(output, heartbeat, BizCode::HEART_BEAT);
	m_tcpClient->setMsgHeartbeat(output);
	m_tcpClient->beginHeartbeat(m_secHeartbeat);
}

int32_t Client::getCodes(int32_t *codes)
{
	while (!m_codesReady)
	{
		this_thread::sleep_for(chrono::seconds(1));
	}
	size_t i = 0;
	for (const auto code : m_codes)
	{
		codes[i++] = code;
	}
	return int32_t(i);
}

void Client::addReduceCode(int8_t addReduce, int32_t *codes, int32_t size)
{
	PbCodesSub pbMsg;
	if (addReduce)
	{
		pbMsg.set_add_red(true);
	}
	else
	{
		pbMsg.set_add_red(false);
	}
	for (int32_t i = 0; i < size; ++i)
	{
		//if (m_codes.find(codes[i]) != m_codes.end()) {
		pbMsg.add_codes(codes[i]);
		//}
	}
	async_send(pbMsg, BizCode::CODES_SUB);
}


void Client::getBizcode(BizCode &bizCode, string &pbmsg, const string &msgInput)
{
	bizCode = BizCode((unsigned char) (msgInput[0]));
	pbmsg = msgInput.substr(1, string::npos);
}

void Client::addBizcode(string &output, const google::protobuf::Message &pbmsg, BizCode bizCode)
{
	string msg;
	pbmsg.SerializeToString(&msg);
	char strBizCode[2] = { 0 };
	strBizCode[0] = bizCode;
	output = string(strBizCode) + msg;
}

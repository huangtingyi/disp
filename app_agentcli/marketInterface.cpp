#include <boost/property_tree/json_parser.hpp>
#include <boost/asio.hpp>
#include "TcpClient.h"
#include "Client.h"

#include "domain2ipPort.h"

#include "marketInterface.h"

static boost::asio::io_service *io_ser = nullptr;
static Client *client = nullptr;

thread *pThread = nullptr;

static bool connected = false;
static void *st_des;

void threadFuncTcpclientStart(bool &createIos, bool &connected)
{
	if (!createIos)
	{
		io_ser = new boost::asio::io_service;
	}
	createIos = true;
	//while (!connected) {
	//this_thread::sleep_for(chrono::milliseconds(300));
	//}
	{
		//shared_ptr<boost::asio::io_service::work> w = make_shared<boost::asio::io_service::work>(boost::asio::io_service::work(*io_ser));
		boost::asio::io_service::work *w = new boost::asio::io_service::work(boost::asio::io_service::work(*io_ser));
		io_ser->run();
		delete w;
		//BOOST_LOG_SEV(g_lg, trace) << __FUNCTION__ << " io_ser has stoptted";
	}
	io_ser->stop();
	delete io_ser;
	io_ser = nullptr;
	createIos = false;
	//BOOST_LOG_SEV(g_lg, trace) << __FUNCTION__ << " will finish";
}
int16_t m_connect(char *errmsg)
{
	static bool createIos = false;
	//connected = false;
	pThread = new /*make_shared<thread>*/thread(threadFuncTcpclientStart, std::ref(createIos), std::ref(connected));
	do
	{
		this_thread::sleep_for(chrono::seconds(1));
	} while (createIos == false);
	TcpClient *tcpclient = new TcpClient(*io_ser);
	tcpclient->m_ip = g_strServer.ip;
	tcpclient->m_port = g_iPort;
	client = new Client;

	client->m_connected = &connected;
	client->setTcpClient(tcpclient);
	string errmsgTmp;
	if (client->connect(errmsgTmp))
	{
		//BOOST_LOG_SEV(g_lg, info) << "connect success";
		return MarketInterfaceError::M_SUCCESS;
	}
	else
	{
		strncpy(errmsg, errmsgTmp.c_str(), 255 < (1 + errmsgTmp.size()) ? 255 : (1 + errmsgTmp.size()));
		//BOOST_LOG_SEV(g_lg, error) << "connect FAIL " << errmsg;
		////BOOST_LOG_SEV(g_lg, trace) << "before marketInterfaceClose";
		marketInterfaceClose();
		////BOOST_LOG_SEV(g_lg, trace) << __FUNCTION__ << " before return";
		return MarketInterfaceError::ERR_CONNECT;
	}
}

int16_t login(char *errmsg)
{
	client->setUser(g_strUsername);
	client->setPassword(g_strPassword);
	client->m_loginSuccess = UINT8_MAX;
	client->login();

	do
	{
		this_thread::sleep_for(chrono::seconds(1));
	} while (client->m_loginSuccess == UINT8_MAX);

	if (client->m_loginSuccess == M_SUCCESS)
		return M_SUCCESS;

	strncpy(errmsg, client->m_loginErrMsg.c_str(), 255);
	//marketInterfaceClose();
	return ERR_LOGIN;
}

void subscribe(bool marketdata, bool transaction, bool orderqueue, bool order)
{
	client->subscribeReq(marketdata, transaction, orderqueue, order);
}

void replay(bool marketdata, bool transaction, bool orderqueue, bool order, uint32_t timeBegin, uint32_t timeEnd, uint32_t date)
{
	client->yyyymmddReplay = date;
	client->subscribeReq(marketdata, transaction, orderqueue, order, true, timeBegin, timeEnd, date);
}

void marketInterfaceCloseImpl()
{
	this_thread::sleep_for(chrono::seconds(2));
	if (client){
		client->m_loginSuccess = UINT8_MAX;
		client->stop();
		if (client->m_tcpClient)
			delete client->m_tcpClient;

		delete client;
		client = nullptr;
	}
	//BOOST_LOG_SEV(g_lg, trace) << "before io_ser";
	if (io_ser)
	{
		io_ser->stop();
	}
	//µ»¥˝threadFuncTcpclientStart÷’÷π
	//this_thread::sleep_for(chrono::seconds(1));
	//BOOST_LOG_SEV(g_lg, trace) << __FUNCTION__ << " will complete";
	if (pThread)
	{
		if (pThread->joinable())
		{
			pThread->join();
			delete pThread;
			pThread = nullptr;
		}
	}
}

void marketInterfaceClose()
{
	thread t(marketInterfaceCloseImpl);
	t.detach();
}

int32_t getCodes(int32_t *codes)
{
	return client->getCodes(codes);
}

void addReduceCode(int8_t addReduce, int32_t *codes, int32_t size)
{
	client->addReduceCode(addReduce, codes, size);
}

void setDes(void *d)
{
	st_des = d;
}

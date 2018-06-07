#ifndef __CALL_BACK_BASE_H__
#define __CALL_BACK_BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <signal.h>
#include <thread>
#include <time.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <string>
#include <vector>
#include <queue>
#include <memory>

#include "wwtiny.h"
#include "mktdata.pb.h"
#include "d31data.pb.h"

using namespace std;
//using namespace boost;
//using namespace boost::asio;

namespace PB = google::protobuf;
namespace BIP = boost::asio::ip;
namespace BIO = boost::asio;
namespace BSYS = boost::system;

class CallBackBase
{
public:
	CallBackBase(char sIp[],int iPort,char sUserName[],char sPassword[],
		int iMaxCnt,char sTypeList[]);
	~CallBackBase(){};
	void Login();
	int Connect();
	void SubscribeAll();
	void SubscribeNo();
	void DisplayStat(BizCode iBizCode);
	bool 		m_exit;
	int		m_iTotalCnt;
private:
	void handle_read(const BSYS::error_code& err, size_t byte_transferred);
	void handle_error(const BSYS::error_code& err);
	void handle_write(const BSYS::error_code& err, size_t byte_transferred);
	void handle_connect(bool c = true);
	void AsyncSend(const PB::Message &msg, BizCode bizCode);
	void Recv(string &msg);
	void toHeartbeat(const BSYS::error_code& errcode);
	void beginHeartbeat();
	thread *	m_pThread;
	
	char   m_sTypeList[32];
	string m_username;
	string m_password;
	string m_ip;
	int m_port;
	
	bool		m_loginSuccess;
	bool 		m_connect;
	bool 		m_readHeader;
	unsigned int 	m_lenBody;
	
	int		m_maxcnt;
	int		m_secHeartbeat;
	string		m_strHeartbeat;
	BIO::deadline_timer *m_timerHeartbeat;
	

	BIO::io_service *m_ios;
	BIP::tcp::socket *m_socket;
	BIO::streambuf	m_rbuf;
	queue<string> m_que_wbuf;
	
	int		m_iMktCnt;
	int		m_iTraCnt;
	int		m_iQueCnt;
	int		m_iOrdCnt;
	int		m_iD31Cnt;
};

#endif

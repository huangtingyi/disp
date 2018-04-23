#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include "TcpClient.h"
#include "Client.h"

using namespace std;

#define HEARTBEAT_INTERV 10
TcpClient::TcpClient(boost::asio::io_service &io_ser) :
		m_ioSvr(io_ser), m_socket(io_ser), m_timerHeartbeat(io_ser, boost::posix_time::seconds(HEARTBEAT_INTERV)), m_writeImpl2(std::bind(&TcpClient::writeImpl2, this, placeholders::_1))
{
}

bool TcpClient::connect(string &errmsg)
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_ip), m_port);
	boost::system::error_code errcode;
	m_socket.connect(endpoint, errcode);
	if (errcode)
	{
		//cerr << __FUNCTION__ << errcode.message() << endl;
		m_socket.close();
		errmsg = errcode.message();
		return false;
	}
	m_readHeader = true;
	boost::asio::async_read(m_socket, m_streambuf, boost::asio::transfer_exactly(2), boost::bind(&TcpClient::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	handle_connect();
	//m_timerHeartbeat.async_wait(boost::bind(&TcpClient::toHeartbeat, this, boost::asio::placeholders::error));
	return true;
}

void TcpClient::handle_connect(bool c)
{
	m_client->handle_connect(c);
}

void TcpClient::handle_read(const boost::system::error_code& err/*error*/, size_t bytes_transferred/*bytes_transferred*/)
{
	if (!err)
	{
		m_streambuf.commit(bytes_transferred);
		stringstream ss;
		if (m_readHeader)
		{
			if (m_streambuf.size() >= 2)
			{
				ss << &m_streambuf;
				std::string strHeader = ss.str();
				strHeader.resize(2);
				m_lenBody = ntohs(*(u_short*) (strHeader.data()));

				m_streambuf.consume(2);
				//m_recvTotal += 2;
				m_readHeader = false;
				boost::asio::async_read(m_socket, m_streambuf, boost::asio::transfer_exactly(m_lenBody), boost::bind(&TcpClient::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
				//m_socket.async_read_some(m_streambuf, boost::bind(&TcpClient::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
		}
		else
		{
			if (m_streambuf.size() >= m_lenBody)
			{
				//time_t timeNow = time(nullptr);
				//struct tm* tmNow = localtime(&timeNow);
				//m_recvTotal += m_lenBody;

				ss << &m_streambuf;
				string msg = ss.str();
				msg.resize(m_lenBody);
				m_client->recv(msg);

				m_streambuf.consume(m_lenBody);
				m_readHeader = true;
				boost::asio::async_read(m_socket, m_streambuf, boost::asio::transfer_exactly(2), boost::bind(&TcpClient::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
		}
	}
	else
		handle_error(err);
}

//void TcpClient::write(const string &msg) {
//	auto msgSend = makeMsgSend(msg);
//	writeImpl(std::move(msgSend));
//}

void TcpClient::writeThreadSafe(const string &msg)
{
	auto msgSend = makeMsgSend(msg);
	m_ioSvr.post(std::bind(m_writeImpl2, msgSend));
}

void TcpClient::handle_write(const boost::system::error_code& err, size_t byte_transferred)
{
	if (err)
	{
		handle_error(err);
	}
	else
	{
		m_buf.pop();
	}
}

void TcpClient::toHeartbeat(const boost::system::error_code& errcode)
{
	if (!errcode)
	{
		m_buf.push(m_msgHeartbeat);
		boost::asio::async_write(m_socket, boost::asio::buffer(*m_msgHeartbeat), boost::bind(&TcpClient::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		m_timerHeartbeat.expires_from_now(boost::posix_time::seconds(m_secHeartbeat));
		m_timerHeartbeat.async_wait(boost::bind(&TcpClient::toHeartbeat, this, boost::asio::placeholders::error));
	}
	else
	{

	}
}

void TcpClient::setMsgHeartbeat(const string &msg)
{
	unsigned locallenBody = unsigned(msg.size());
	u_short networkLenBody = htons(locallenBody);
	m_msgHeartbeat = make_shared < vector<char> > (locallenBody + 2);
	memcpy(m_msgHeartbeat->data(), &networkLenBody, 2);
	memcpy(m_msgHeartbeat->data() + 2, msg.data(), locallenBody);
}

void TcpClient::handle_error(const boost::system::error_code& err)
{
	//cerr << err.message() << endl;
	m_client->handle_error(err.message());
}

void TcpClient::beginHeartbeat(unsigned secHeartbeat)
{
	m_secHeartbeat = secHeartbeat;
	TcpClient::toHeartbeat(boost::system::error_code());
}

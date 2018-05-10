#pragma once
#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
using namespace std;

class Client;

class TcpClient
{
public:
	TcpClient(boost::asio::io_service &io_ser);
	boost::asio::io_service &m_ioSvr;
	//return true:³É¹¦ false:Ê§°Ü
	bool connect(string &errmsg);
	inline void setClient(Client *client)
	{
		m_client = client;
	}
	;
	void setMsgHeartbeat(const string &msg);
	inline void stop()
	{
		m_timerHeartbeat.cancel();
		m_socket.close();
		m_streambuf.consume(m_streambuf.size());
	}
	;
	string m_ip;
	uint16_t m_port;
	void beginHeartbeat(unsigned secHeartbeat);
	void writeThreadSafe(const string &msg);

private:
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::streambuf m_streambuf;
	//void write(const string &msg);
	void handle_read(const boost::system::error_code& err, size_t byte_transferred);
	void handle_write(const boost::system::error_code& err, size_t byte_transferred);
	void handle_error(const boost::system::error_code& err);
	void handle_connect(bool c = true);
	bool m_readHeader = true;
	unsigned m_lenBody;
	//unsigned m_recvTotal = 0;
	Client *m_client;
	boost::asio::deadline_timer m_timerHeartbeat;
	void toHeartbeat(const boost::system::error_code& errcode);
	shared_ptr<vector<char> > m_msgHeartbeat;
	unsigned m_secHeartbeat;
	queue<shared_ptr<vector<char> > > m_buf;
	inline shared_ptr<vector<char> > makeMsgSend(const string &msg)
	{
		unsigned locallenBody = unsigned(msg.size());
		u_short networkLenBody = htons(locallenBody);
		auto msgSend = make_shared < vector<char> > (2 + msg.size());
		(*msgSend)[0] = *(char*) &networkLenBody;
		(*msgSend)[1] = *((char*) &networkLenBody + 1);
		memcpy(msgSend->data() + 2, msg.c_str(), msg.size());
		return std::move(msgSend);
	}
	inline void writeImpl(shared_ptr<vector<char> > &msgSend)
	{
		m_buf.push(msgSend);
		boost::asio::async_write(m_socket, boost::asio::buffer(*msgSend, msgSend->size()), 
			boost::bind(&TcpClient::handle_write, this, boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred));
	}
	;
	inline void writeImpl2(shared_ptr<vector<char> > msgSend)
	{
		writeImpl(msgSend);
	}
	;
	std::function<void(shared_ptr<vector<char> >)> m_writeImpl2;
};

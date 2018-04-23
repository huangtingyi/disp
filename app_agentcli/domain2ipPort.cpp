//#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "domain2ipPort.h"

shared_ptr<boost::asio::ip::tcp::endpoint> domain2ipPort::trans(const string &domain, const uint16_t port)
{
	char strPort[6] = { 0 };
	snprintf(strPort, 6, "%u", port);
	return domain2ipPort::trans(domain, strPort);
}

shared_ptr<boost::asio::ip::tcp::endpoint> domain2ipPort::trans(const string &domain, const string &port)
{
	boost::asio::io_service ioSvr;
	boost::asio::ip::tcp::resolver resolver(ioSvr);
	boost::asio::ip::tcp::resolver::query query(domain, port);

	boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
	boost::asio::ip::tcp::resolver::iterator end; // End marker.  

	//while (iter != end)
	//{
	boost::asio::ip::tcp::endpoint endpoint = *iter++;
	ioSvr.stop();
	//std::cout << endpoint << std::endl;
	return make_shared < boost::asio::ip::tcp::endpoint > (endpoint);
	//}
}

#pragma once
#include <string>
#include <memory>
#include <iostream>

using namespace std;
namespace boost
{
namespace asio
{
namespace ip
{
//class tcp {
class endpoint;
//}
}
}
}

class domain2ipPort
{
public:
	static shared_ptr<boost::asio::ip::tcp::endpoint> trans(const string &domain, const uint16_t port);
	static shared_ptr<boost::asio::ip::tcp::endpoint> trans(const string &domain, const string &port);
};

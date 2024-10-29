#include "Client.hpp"
#include "Logger.hpp"

Client::Client()
{
	Logger::getInstance().log(INFO, "\e[30;101mclient created\e[0m", *this);
	
	fd = 0;		
	memset(&address, 0, sizeof(address));
	addressLen = sizeof(address);
	
	bodySize = 0;
	messageSend.reserve(MAX_HDR_SIZE);
	messageSend.resize(SEND_BUFF_SIZE);
	respHeader = false;
headerRespons.reserve(300); //!
messageRecv.reserve(300); //!
	std::stringstream ss;		
	ss << 
	"HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: 316\r\n\
Connection: close\r\n\
\r\n\
"; 
	std::string str = ss.str();
	std::vector<char> res(str.begin(), str.end());	
	headerRespons = res;
	ping = true;	
}
Client::Client(const Client & src)
{
	Logger::getInstance().log(INFO, "\e[30;101mclient copy created\e[0m", *this);

	*this = src;
}
Client::~Client()
{
	Logger::getInstance().log(INFO, "\e[30;101mclient deleted\e[0m", *this);
}

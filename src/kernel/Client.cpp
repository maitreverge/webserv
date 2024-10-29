#include "Client.hpp"
#include "Logger.hpp"

Client::Client()
{
	Logger::getInstance().log(INFO, "\e[30;101mclient created\e[0m", *this);
	
	fd = 0;		
	memset(&address, 0, sizeof(address));
	addressLen = sizeof(address);
	
	messageRecv.reserve(300); //!
	bodySize = 0;

	headerRespons.reserve(300); //!
	messageSend.reserve(MAX_HDR_SIZE);
	messageSend.resize(SEND_BUFF_SIZE);
	
	ping = true;	
	pongHeader = false;
}

Client::Client(const Client & src)
{
	Logger::getInstance().log(INFO, "\e[30;101mclient copy created\e[0m",
		*this);

	*this = src;
}

Client::~Client()
{
	Logger::getInstance().log(INFO, "\e[30;101mclient deleted\e[0m",
		*this);
}

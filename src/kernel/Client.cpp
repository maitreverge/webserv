#include "Client.hpp"
#include "Logger.hpp"

Client::Client()
{
	Logger::getInstance().log(INFO, "\e[30;101mclient created\e[0m", *this);
	
	fd = 0;		
	memset(&address, 0, sizeof(address));
	addressLen = sizeof(address);
	
	messageRecv.reserve(MAX_HDR_SIZE); 
	bodySize = 0;

	headerRespons.reserve(MAX_HDR_SIZE); 
	messageSend.reserve(SEND_BUFF_SIZE);
	
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

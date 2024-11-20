#include "Client.hpp"
#include "Logger.hpp"

Client::Client(Config * _conf) : conf(_conf)
{
	// Logger::getInstance().log(INFO, "\e[30;101mclient created\e[0m", *this);
	
	fd = 0;		
	memset(&address, 0, sizeof(address));
	addressLen = sizeof(address);
	
	messageRecv.reserve(static_cast<size_t>(_conf->maxHeaderSize)); 
	bodySize = 0;
	chunkedSize = -1;
	headerRespons.reserve(static_cast<size_t>(_conf->maxHeaderSize)); 
	messageSend.reserve(SEND_BUFF_SIZE);
	
	ping = true;	
	pongHeader = false;
	exitRequired = false;
}

#include "Client.hpp"
#include "Logger.hpp"

Client::Client(Config * _conf) : conf(_conf)
{
	// Logger::getInstance().log(INFO, "\e[30;101mclient created\e[0m", *this);
	
	fd = 0;		
	std::memset(&address, 0, sizeof(address));
	addressLen = sizeof(address);
	
	messageRecv.reserve(static_cast<size_t>(_conf->maxHeaderSize)); 
	bodySize = 0;
	chunkSize = -1;
	retryChunked = false;
	headerRespons.reserve(static_cast<size_t>(_conf->maxHeaderSize)); 
	messageSend.reserve(conf->send_buff_size);
	sendBuffer.reserve(_conf->maxBodySize);
	sendFlag = false;
	
	ping = true;	
	pongHeader = false;
	exitRequired = false;
}

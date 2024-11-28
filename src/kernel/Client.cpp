#include "Client.hpp"
#include "Logger.hpp"

Client::Client(Config * _conf, Server* server) : conf(_conf), _server(server)
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

bool Client::isConnected() const
{
	CookiesMap::const_iterator itCookie = cookies._cookies.find("sessionID");
	if (itCookie == cookies._cookies.end())
		return (printColor(RED, "FIRST"), false);
	if (!_server)
		return (printColor(RED, "SECOND"), false);
	const std::string& token = itCookie->second;
	if (_server->UserSessions.find(token) != _server->UserSessions.end())
		return true;
	return (printColor(RED, "THIRD"), false);

}


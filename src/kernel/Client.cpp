/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:59 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:10:00 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Logger.hpp"

Client::Client(Config * _conf, Server* server) : conf(_conf), _server(server)
{
	fd = -1;		
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

Client::Client(const Client & src)
{
	this->fd = -1;
	messageRecv.reserve(static_cast<size_t>(src.conf->maxHeaderSize));
	headerRespons.reserve(static_cast<size_t>(src.conf->maxHeaderSize)); 
	messageSend.reserve(src.conf->send_buff_size);
	sendBuffer.reserve(src.conf->maxBodySize);
	*this = src;
}

Client & Client::operator=(const Client & rhs)
{
	this->clone(rhs);
	if (this->fd >= 0)
	{
		FD_CLR(this->fd, &Kernel::_actualSet);
		close(this->fd);
	}
	if (rhs.fd >= 0)
	{
		if ((this->fd = dup(rhs.fd)) >= 0)
			FD_SET(this->fd, &Kernel::_actualSet);
		Kernel::_maxFd = std::max(Kernel::_maxFd, this->fd);
	}
	else
		this->fd = rhs.fd;			
	return *this;
}

void Client::clone(const Client & rhs)
{
	this->conf = rhs.conf;
	this->address = rhs.address;
	this->addressLen = rhs.addressLen;

	this->messageRecv = rhs.messageRecv;
	this->headerRequest = rhs.headerRequest;
	this->bodySize = rhs.bodySize;
	this->chunkSize = rhs.chunkSize;
	this->retryChunked = rhs.retryChunked;
	
	this->responseBuilder = rhs.responseBuilder;
	this->headerRespons = rhs.headerRespons;
	this->messageSend = rhs.messageSend;
	this->sendBuffer = rhs.sendBuffer;
	this->sendFlag = rhs.sendFlag;
	
	this->ping = rhs.ping;
	this->pongHeader = rhs.pongHeader;
	this->exitRequired = rhs.exitRequired;

	this->cookies = rhs.cookies;
	this->_server = rhs._server;
}

Client::~Client()
{
	if (this->fd < 0)
		return ;	
	FD_CLR(this->fd, &Kernel::_actualSet);
	close(this->fd);
}
bool Client::isConnected() const
{
	CookiesMap::const_iterator itCookie = cookies._cookies.find("sessionID");
	if (itCookie == cookies._cookies.end())
		return (false);
	if (!_server)
		return (false);
	const std::string& token = itCookie->second;
	if (_server->UserSessions.find(token) != _server->UserSessions.end())
		return true;
	return (false);

}


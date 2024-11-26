#include "Client.hpp"
#include "Logger.hpp"

Client::Client(Config * _conf) : conf(_conf)
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
	// messageRecv.reserve(static_cast<size_t>(src.conf->maxHeaderSize));
	// headerRespons.reserve(static_cast<size_t>(src.conf->maxHeaderSize)); 
	// messageSend.reserve(src.conf->send_buff_size);
	// sendBuffer.reserve(src.conf->maxBodySize);
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
}

Client::~Client()
{
	if (this->fd < 0)
		return ;	
	FD_CLR(this->fd, &Kernel::_actualSet);
	close(this->fd);
}

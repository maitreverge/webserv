#include "Server.hpp"
#include "Logger.hpp"

void Server::bodyCheckin(const size_t i, const size_t addBodysize)
{
	stringstream ss;
	ss << "Body Checkin - recv " << addBodysize << " bytes";
	Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);
	Server::printVector(this->_clients[i].messageRecv);

	this->_clients[i].bodySize += addBodysize;
	if (this->isChunked(i))
		return;
	if (!this->isContentLengthValid(i)	|| this->isBodyTooLarge(i))	
		return ;
	this->isBodyEnd(i) ? this->sendBodyEnd(i) :	this->sendBodyPart(i);
}

bool Server::isContentLengthValid(const size_t i)
{	
	if (this->_clients[i].headerRequest.getHeaders().ContentLength
		> this->_conf.maxBodySize)
	{			
		stringstream ss;
		ss << "max content size reached" << " - Content-Lenght: "
			<< this->_clients[i].headerRequest.getHeaders().ContentLength
			<< " - Max content size: " << this->_conf.maxBodySize << std::endl;
		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);
			//! 413 Payload Too Large
		this->shortCircuit(CODE_413_PAYLOAD_TOO_LARGE, i);
		return false;
	}
	return true;
}

bool Server::isBodyTooLarge(const size_t i)
{
	if (this->_clients[i].bodySize >
		this->_clients[i].headerRequest.getHeaders().ContentLength)
	{	
		stringstream ss;
		ss << "content size" << " - Body-Size: "
		    << this->_clients[i].bodySize << " Content-Lenght: "
		    << this->_clients[i].headerRequest.getHeaders().ContentLength
            << std::endl;
		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);
			//! 413 Payload Too Large
		this->shortCircuit(CODE_413_PAYLOAD_TOO_LARGE, i);
		return true;;
	}
	return false;
}

bool Server::isBodyEnd(const size_t i)
{
	Logger::getInstance().log(INFO, "Is Body End", this->_clients[i]);

	if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
		== "chunked" && !this->_clients[i].chunkSize)
	{
		stringstream ss; ss << "client body terminated" << " - Chunked-Size: "
		<< this->_clients[i].chunkSize;
		Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);
		return true;
	}
	else if (this->_clients[i].bodySize ==
		this->_clients[i].headerRequest.getHeaders().ContentLength)
	{
		stringstream ss; ss << "client body terminated" << " - Body-Size: "
		<< this->_clients[i].bodySize << " Content-Lenght: "
		<< this->_clients[i].headerRequest.getHeaders().ContentLength;
		Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);
		return true;
	}
	return false;
}

void Server::sendBodyPart(const size_t i)
{
	Logger::getInstance().log(INFO, "Send Body Part", this->_clients[i]);
	Server::printVector(this->_clients[i].messageRecv);
	
	if (this->_clients[i].headerRequest.getMethod() == "POST")
	{
		try {
			this->_clients[i].responseBuilder.
				setBodyPost(this->_clients[i], false);	}
		catch(const Server::ShortCircuitException& e)
		{	this->shortCircuit(e.getCode(), i);	}
	}
	else
		this->_clients[i].messageRecv.clear();	
}

void Server::sendBodyEnd(const size_t i)
{	
	Logger::getInstance().log(INFO, "Send Body End", this->_clients[i]);
	Server::printVector(this->_clients[i].messageRecv);
	
	if (this->_clients[i].headerRequest.getMethod() == "POST")
	{
		try {
			this->_clients[i].responseBuilder.
				setBodyPost(this->_clients[i], true);	}
		catch(const Server::ShortCircuitException& e)
		{	this->shortCircuit(e.getCode(), i);	}	
	}					
	else
	{
		this->_clients[i].messageRecv.clear();
		shutdown(this->_clients[i].responseBuilder._cgi._fds[1], SHUT_WR);
	}
	if (this->_clients[i].messageRecv.empty())
		this->_clients[i].ping = false;	
}

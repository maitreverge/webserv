#include "Server.hpp"
#include "Logger.hpp"

void Server::bodyCheckin(const size_t i, const size_t addBodysize)
{
	std::stringstream ss;
	ss << "Body Checkin - recv " << addBodysize << " bytes";
	Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);
	Server::printVector(this->_clients[i], this->_clients[i].messageRecv);

	this->_clients[i].bodySize += addBodysize;
	if (this->isChunked(i))
		return;
	this->isContentLengthValid(i);
	this->isBodyTooLarge(i);	
	this->isBodyEnd(i) ? this->sendBodyEnd(i) :	this->sendBodyPart(i);
}

void Server::isContentLengthValid(const size_t i)
{	
	if (this->_clients[i].headerRequest.getHeaders().ContentLength
		> this->_conf.maxBodySize)
	{			
		std::stringstream ss;
		ss << "max content size reached" << " - Content-Lenght: "
			<< this->_clients[i].headerRequest.getHeaders().ContentLength
			<< " - Max content size: " << this->_conf.maxBodySize << std::endl;
		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);

		throw Server::ShortCircuitException(CODE_413_PAYLOAD_TOO_LARGE);
	}
}

void Server::isBodyTooLarge(const size_t i)
{
	if (this->_clients[i].bodySize >
		this->_clients[i].headerRequest.getHeaders().ContentLength)
	{	
		std::stringstream ss;
		ss << "content size" << " - Body-Size: "
		    << this->_clients[i].bodySize << " Content-Lenght: "
		    << this->_clients[i].headerRequest.getHeaders().ContentLength
            << std::endl;
		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);	

		throw Server::ShortCircuitException(CODE_413_PAYLOAD_TOO_LARGE);
	}
}

bool Server::isBodyEnd(const size_t i)
{
	Logger::getInstance().log(DEBUG, "Is Body End", this->_clients[i]);

	if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
		== "chunked" && !this->_clients[i].chunkSize)
	{
		std::stringstream ss;
		ss << "client body terminated" << " - Chunked-Size: "
		<< this->_clients[i].chunkSize;
		Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);
		return true;
	}
	else if (this->_clients[i].bodySize ==
		this->_clients[i].headerRequest.getHeaders().ContentLength)
	{
		std::stringstream ss;
		ss << "client body terminated" << " - Body-Size: "
		<< this->_clients[i].bodySize << " Content-Lenght: "
		<< this->_clients[i].headerRequest.getHeaders().ContentLength;
		Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);
		return true;
	}
	return false;
}

void Server::sendBodyPart(const size_t i)
{
	Logger::getInstance().log(DEBUG, "Send Body Part", this->_clients[i]);
	Server::printVector(this->_clients[i], this->_clients[i].messageRecv);
	
	if (this->_clients[i].headerRequest.getMethod() == "POST")
		this->_clients[i].responseBuilder.setBody(this->_clients[i], false);		
	else
		this->_clients[i].messageRecv.clear();	
}

void Server::sendBodyEnd(const size_t i)
{	
	Logger::getInstance().log(DEBUG, "Send Body End", this->_clients[i]);
	Server::printVector(this->_clients[i], this->_clients[i].messageRecv);
	
	if (this->_clients[i].headerRequest.getMethod() == "POST")		
		this->_clients[i].responseBuilder.setBody(this->_clients[i], true);						
	else
	{
		this->_clients[i].messageRecv.clear();
		if (this->_clients[i].responseBuilder._cgi._fds[1] > 0 && 
			shutdown(this->_clients[i].responseBuilder._cgi._fds[1], SHUT_WR)
			< 0)
			throw (Logger::getInstance().log(ERROR, "setbody shutdown",
				this->_clients[i]),
				Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
	}
	if (this->_clients[i].messageRecv.empty())
		this->_clients[i].ping = false;	
}

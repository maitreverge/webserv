#include "Server.hpp"
#include "Logger.hpp"

void Server::listenClients()
{	
	for (size_t i = 0; i < this->_clients.size(); i++)
	{			
		if (!this->_clients[i].ping)
			continue ;
		if (!this->_clients[i].messageRecv.empty() 
			&& this->_clients[i].headerRequest.getHeaders().ContentLength)				
			this->retrySend(i);
		else if (this->_clients[i].retryChunked)
			this->isChunked(i);
		else if (this->recevData(i))
			break ;
	}
}

bool Server::recevData(const size_t i)
{
	if (FD_ISSET(this->_clients[i].fd, &Kernel::_readSet))
	{
		this->_readBuffer.clear();
		this->_readBuffer.resize(RECV_BUFF_SIZE);	
		Logger::getInstance().log(WARNING, "ready to recv",
			this->_clients[i]);		
		ssize_t ret = recv(this->_clients[i].fd, this->_readBuffer.data(),
			this->_readBuffer.size(), 0);
		Kernel::cleanFdSet(this->_clients[i]);		
		if (ret <= 0)
		{
			if (ret < 0)
				Logger::getInstance().log(ERROR, "recv", this->_clients[i]);
			this->exitClient(i);
			return true;
		}				
		else
			this->clientMessage(i, ret);			
	}	
	return false;
}

void Server::retrySend(const size_t i)
{
	Logger::getInstance().log(INFO, "Retry Send", this->_clients[i]);	
	stringstream ss; ss << "Content-Length: "
		<< this->_clients[i].headerRequest.getHeaders().ContentLength
		<< " MessageRecv-Size: " << this->_clients[i].messageRecv.size()
		<< std::endl;
	Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);

	this->isBodyEnd(i) ? this->sendBodyEnd(i) :	this->sendBodyPart(i);
}

void Server::clientMessage(const size_t i, ssize_t ret)
{
	this->_clients[i].messageRecv.
	insert(this->_clients[i].messageRecv.end(), 
	this->_readBuffer.begin(),
	this->_readBuffer.begin() + ret);				
	if (!this->_clients[i].headerRequest.getHeaders().ContentLength
		&& this->_clients[i].headerRequest.getHeaders().TransferEncoding
		!= "chunked")
		this->headerCheckin(i, ret);
	else
		this->bodyCheckin(i, ret);	
}

bool Server::isDelimiterFind(std::string delimiter, const size_t i,
	std::vector<char>::iterator & it)
{	
	it = std::search
		(this->_clients[i].messageRecv.begin(),
		this->_clients[i].messageRecv.end(),
		delimiter.begin(),
		delimiter.end());
	return (it != this->_clients[i].messageRecv.end());	
}

void Server::headerCheckin(const size_t i, ssize_t ret)
{
	stringstream ss;
	ss << "Header Checkin - recv " << ret << " bytes";
	Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);
	Server::printVector(this->_clients[i].messageRecv);

	std::vector<char>::iterator it;		
	if (this->isDelimiterFind("\r\n\r\n", i, it))		
	{	
		Logger::getInstance().log(DEBUG, "header terminated",
			this->_clients[i]);		
		if (this->isMaxHeaderSize(it + 4, i))
			return ;				
		this->_clients[i].headerRequest.parse(this->_clients[i]);								
		this->_clients[i].headerRequest.displayParsingResult();
		if (!this->_clients[i].headerRequest.getIsValid())		
			return this->shortCircuit(static_cast<e_errorCodes>(400), i);//! FIND REAL ERROR			
		this->getRespHeader(i);
		this->_clients[i].messageRecv.
            erase(this->_clients[i].messageRecv.begin(), it + 4);					
		this->bodyCheckin(i, this->_clients[i].messageRecv.size());						
	}
	else
		this->isMaxHeaderSize(it + 1, i);
}

void Server::getRespHeader(const size_t i)
{
	if (this->_clients[i].headerRequest.getMethod() == "GET")		
		this->_clients[i].responseBuilder.getHeader(this->_clients[i],
			this->_conf);
	else if (this->_clients[i].headerRequest.getMethod() == "POST")		
		this->_clients[i].responseBuilder.getHeaderPost(this->_clients[i],
			this->_conf);
	else if (this->_clients[i].headerRequest.getMethod() == "DELETE")		
		this->_clients[i].responseBuilder.getHeader(this->_clients[i],
			this->_conf);
	else
		this->_clients[i].responseBuilder.getHeader(this->_clients[i],
			this->_conf); 
}

bool Server::isMaxHeaderSize(std::vector<char>::iterator it, const size_t i)
{
	if (it - this->_clients[i].messageRecv.begin() > this->_conf.maxHeaderSize)	
	{		
		stringstream ss;
		ss << "header size" << " Actual-Size: "
            << it - this->_clients[i].messageRecv.begin()
            << " - Max-Header-Size : " << this->_conf.maxHeaderSize;

		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);		
			//! 431 Request Header Fields Too Large !!
		this->shortCircuit(CODE_431_REQUEST_HEADER_FIELDS_TOO_LARGE, i);	
		return true;	
	}
	return false;
}


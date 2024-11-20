#include "Server.hpp"
#include "Logger.hpp"

void Server::listenClients()
{	
	for (size_t i = 0; i < this->_clients.size(); i++)
	{			
		if (!this->_clients[i].ping)
			continue ;
		if ((this->_clients[i].headerRequest.getHeaders().ContentLength)
			&& !this->_clients[i].messageRecv.empty())
			retrySend(i);
		else if (FD_ISSET(this->_clients[i].fd, &Kernel::_readSet))
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
				break ;
			}				
			else
				clientMessage(i, ret);			
		}	
		else if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
			== "chunked" && !this->_clients[i].messageRecv.empty())
			this->isChunked(i);
	}
}

void Server::retrySend(size_t i)
{
	Logger::getInstance().log(INFO, "Re Send", this->_clients[i]);	
	stringstream ss; ss << "Content-Length: "
		<< this->_clients[i].headerRequest.getHeaders().ContentLength
		<< " MessageRecv-Size: " << this->_clients[i].messageRecv.size()
		<< std::endl;
	Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);

	if (this->isBodyEnd(i))
		this->sendBodyEnd(i);
	else
		this->sendBodyPart(i);
}

void Server::clientMessage(size_t i, ssize_t ret)
{
	this->_clients[i].messageRecv.
	insert(this->_clients[i].messageRecv.end(), 
	this->_readBuffer.begin(),
	this->_readBuffer.begin() + ret);				
	if (!this->_clients[i].headerRequest.getHeaders().ContentLength && this->_clients[i].headerRequest.getHeaders().TransferEncoding != "chunked")
		this->handleClientHeader(i, ret);
	else
		this->handleClientBody(i, ret);	
}

bool Server::isDelimiterFind(std::string delimiter, size_t i,
	std::vector<char>::iterator & it)
{	
	it = std::search
		(this->_clients[i].messageRecv.begin(),
		this->_clients[i].messageRecv.end(),
		delimiter.begin(),
		delimiter.end());
	return (it != this->_clients[i].messageRecv.end());	
}

void Server::handleClientHeader(size_t i, ssize_t ret)
{
	stringstream ss;
	ss << "Handle Client Header - receiv client request " << ret << " bytes";
	Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);
	this->printResponse(this->_clients[i].messageRecv);	
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
		this->_clients[i].bodySize += this->_clients[i].messageRecv.size();	
		if (this->isChunked(i))
			return;
		this->bodyCheckin(i);						
	}
	else
		this->isMaxHeaderSize(it + 1, i);
}

void Server::bodyCheckin(size_t i)
{
	if (!this->_clients[i].headerRequest.getHeaders().ContentLength)
	{
		this->_clients[i].ping = false;
		this->isBodyTooLarge(i);		
	}
	else if (!this->isContentLengthValid(i)
		|| this->isBodyTooLarge(i))	
		return ;
	if (this->isBodyEnd(i))
		this->sendBodyEnd(i);
	else
		this->sendBodyPart(i);
}

void Server::getRespHeader(size_t i)
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

void Server::handleClientBody(size_t i, ssize_t ret)
{	
	stringstream ss;
	ss << "Handle Client Body - receive " << ret << " bytes";
	Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);
	this->printResponse(this->_clients[i].messageRecv);

	this->_clients[i].bodySize += static_cast<size_t>(ret);
	if (this->isChunked(i))
		return ;
	if (this->isBodyTooLarge(i))
		return ;
	if (this->isBodyEnd(i))
		this->sendBodyEnd(i);
	else
		this->sendBodyPart(i);	
}

void Server::shortCircuit(e_errorCodes err, size_t i)
{
	Logger::getInstance().log(INFO, "Short Circuit", this->_clients[i]);

	this->_clients[i].responseBuilder = ResponseBuilder();
	this->_clients[i].responseBuilder.getHeader(this->_clients[i],
		this->_conf, err);
	this->_clients[i].messageRecv.clear();
	this->_clients[i].ping = false;
	this->_clients[i].exitRequired = true;
}

bool Server::isMaxHeaderSize(std::vector<char>::iterator it, size_t i)
{
	if (it - this->_clients[i].messageRecv.begin() > this->_conf.maxHeaderSize)	
	{		
		stringstream ss;
		ss << "header size" << " Actual-Size: "
            << it - this->_clients[i].messageRecv.begin()
            << " - Max-Header-Size : " << this->_conf.maxHeaderSize;

		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);		
			//! 431 Request Header Fields Too Large !!
		this->shortCircuit(static_cast<e_errorCodes>(431), i);	
		return true;	
	}
	return false;
}

bool Server::isContentLengthValid(size_t i)
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
		this->shortCircuit(static_cast<e_errorCodes>(413), i);
		return false;
	}
	return true;
}

bool Server::isBodyTooLarge(size_t i)
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
		this->shortCircuit(static_cast<e_errorCodes>(413), i);
		return true;;
	}
	return false;
}

void Server::sendBodyPart(size_t i)
{
	Logger::getInstance().log(INFO, "send Body Part", this->_clients[i]);
	this->printResponse(this->_clients[i].messageRecv);
	
	if (this->_clients[i].headerRequest.getMethod() == "POST")
	{
		try {
			this->_clients[i].responseBuilder.
				setBodyPost(this->_clients[i], false);	}
		catch(const std::exception& e)
		{	this->shortCircuit(CODE_508_LOOP_DETECTED, i);	}
	}
	else
		this->_clients[i].messageRecv.clear();	
}

void Server::sendBodyEnd(size_t i)
{	
	Logger::getInstance().log(INFO, "Send Body End", this->_clients[i]);
	this->printResponse(this->_clients[i].messageRecv);
	
	if (this->_clients[i].headerRequest.getMethod() == "POST")
	{
		try {
			this->_clients[i].responseBuilder.
				setBodyPost(this->_clients[i], true);	}
		catch(const std::exception& e)
		{	this->shortCircuit(CODE_508_LOOP_DETECTED, i);	}
	}					
	else
	{
		this->_clients[i].messageRecv.clear();
		shutdown(this->_clients[i].responseBuilder._cgi._fds[1], SHUT_WR);
	}
	if (this->_clients[i].messageRecv.empty())
		this->_clients[i].ping = false;	
}

bool Server::isBodyEnd(size_t i)
{
	if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
		== "chunked" && !this->_clients[i].chunkedSize)
	{
		stringstream ss; ss << "client body terminated" << " - Chunked-Size: "
		<< this->_clients[i].chunkedSize;
		Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);
		return true;
	}
	else if (this->_clients[i].bodySize ==
		this->_clients[i].headerRequest.getHeaders().ContentLength)
	{
		stringstream ss; ss << "client body terminated" << " - Body-Size: "
		<< this->_clients[i].bodySize << " Content-Lenght: "
		<< this->_clients[i].headerRequest.getHeaders().ContentLength;
		Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);
		return true;
	}
	return false;
}

bool Server::isChunked(size_t i)
{	
	if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
		== "chunked")
	{	
		Logger::getInstance().log(INFO, "chunked",
			this->_clients[i]);//!
		std::vector<char>::iterator it;
		if (this->_clients[i].chunkedSize >= 0
			|| this->isDelimiterFind("\r\n", i, it))
		{
			Logger::getInstance().log(INFO, "first delim found",
				this->_clients[i]);//!		
			if 	(this->_clients[i].chunkedSize < 0)
			{

				std::string hexaLen(this->_clients[i].messageRecv.begin(), it);
				std::stringstream ss; ss << hexaLen;
				size_t len;
				ss >> len;
				if (!ss)
					Logger::getInstance().log(ERROR, "hexadecimal conversion",
						this->_clients[i]);//!
				else
				{
					stringstream ss; ss << "hexadecimal succes - len: " << len;
					Logger::getInstance().log(DEBUG, ss.str(), 
						this->_clients[i]);//!
					this->_clients[i].chunkedSize = (long)len;
				}
				this->_clients[i].messageRecv.
					erase(this->_clients[i].messageRecv.begin(), it + 2);
			
			}
			if (this->isDelimiterFind("\r\n", i, it))
			{	
				Logger::getInstance().log(DEBUG, "second delim found",
					this->_clients[i]);//!
				stringstream ss; ss << "chunked size "
					<< this->_clients[i].chunkedSize;
				Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);//!
				if (std::distance(this->_clients[i].messageRecv.begin(), it) == static_cast<std::ptrdiff_t>(this->_clients[i].chunkedSize))
				{
					Logger::getInstance().log(DEBUG, "chunk part completed",
						this->_clients[i]);//!
					std::vector<char> tmp(it, this->_clients[i].messageRecv.end());
					this->_clients[i].messageRecv.erase(it, this->_clients[i].messageRecv.end());
					this->_clients[i].chunkedSize = -1;
					if (this->isBodyEnd(i))
						sendBodyEnd(i);
					else
						sendBodyPart(i);
					this->_clients[i].messageRecv.assign(tmp.begin() + 2, tmp.end()); //! ne marchera pas avec resend					
				}
				else if (std::distance(this->_clients[i].messageRecv.begin(), it) > static_cast<std::ptrdiff_t>(this->_clients[i].chunkedSize))
				{
					Logger::getInstance().log(ERROR, "chunk size",
						this->_clients[i]);//!
				}
				else
					Logger::getInstance().log(DEBUG, "chunk part inferior",
						this->_clients[i]);//!
			}
			else
				Logger::getInstance().log(INFO, "second delim not found",
					this->_clients[i]);//!
		}
		else
			Logger::getInstance().log(INFO, "first delim not found",
				this->_clients[i]);//!
		return true;	
	}
	// Logger::getInstance().log(INFO, "not chunked",
	// 		this->_clients[i]);//!
	return false;
}

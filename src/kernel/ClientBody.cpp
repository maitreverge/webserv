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

// bool Server::isChunked(const size_t i)
// {	
// 	if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
// 		== "chunked")
// 	{	
// 		this->_clients[i].retryChunked = false;
// 		Logger::getInstance().log(DEBUG, "chunked", this->_clients[i]);
// 		std::vector<char>::iterator it;
// 		if (this->_clients[i].chunkSize >= 0
// 			|| this->isDelimiterFind("\r\n", i, it))
// 		{
// 			Logger::getInstance().log(DEBUG, "first delim found",
// 				this->_clients[i]);//!		
// 			if 	(this->_clients[i].chunkSize < 0)
// 			{

// 				std::string hexaLen(this->_clients[i].messageRecv.begin(), it);
// 				std::stringstream ss; ss << hexaLen;
// 				ssize_t len;
// 				ss >> len;
// 				if (!ss)
// 					Logger::getInstance().log(ERROR, "hexadecimal conversion",
// 						this->_clients[i]);//!
// 				else
// 				{
// 					stringstream ss; ss << "hexadecimal succes - len: " << len;
// 					Logger::getInstance().log(DEBUG, ss.str(), 
// 						this->_clients[i]);//!
// 					this->_clients[i].chunkSize = len;
// 				}
// 				this->_clients[i].messageRecv.
// 					erase(this->_clients[i].messageRecv.begin(), it + 2);
			
// 			}
// 			if (this->isDelimiterFind("\r\n", i, it))
// 			{	
// 				Logger::getInstance().log(DEBUG, "second delim found",
// 					this->_clients[i]);//!
// 				stringstream ss; ss << "chunked size "
// 					<< this->_clients[i].chunkSize;
// 				Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);//!

// 				if (std::distance(this->_clients[i].messageRecv.begin(), it)
// 					== static_cast<std::ptrdiff_t>(this->_clients[i].chunkSize))
// 				{
// 					Logger::getInstance().log(DEBUG, "chunk part completed",
// 						this->_clients[i]);//!
// 					std::vector<char> tmp(it, this->_clients[i].messageRecv.end());
// 					this->_clients[i].messageRecv.erase(it, this->_clients[i].messageRecv.end());
// 					const size_t tmp_size = this->_clients[i].messageRecv.size();

// 					this->isBodyEnd(i) ? this->sendBodyEnd(i) :	this->sendBodyPart(i);

// 					// this->_clients[i].chunkSize = this->_clients[i].messageRecv.empty() ? -1 : tmp_size - this->_clients[i].messageRecv.size();
				
// 					this->_clients[i].chunkSize -= tmp_size - this->_clients[i].messageRecv.size();
// 					if (!this->_clients[i].chunkSize)
// 						this->_clients[i].chunkSize = -1;
									
// 					int clearNLine = this->_clients[i].messageRecv.empty() ?
// 						2 : (this->_clients[i].retryChunked = true, 0);
// 					this->_clients[i].messageRecv.insert(this->_clients[i].messageRecv.end(),
// 						tmp.begin() + clearNLine, tmp.end()); 
// 					Server::printVector(this->_clients[i].messageRecv);	
// 					if (clearNLine == 2 && !this->_clients[i].messageRecv.empty())
// 						this->_clients[i].retryChunked = true;
// 				}
// 				else if (std::distance(this->_clients[i].messageRecv.begin(), it) > static_cast<std::ptrdiff_t>(this->_clients[i].chunkSize))
// 				{
// 					Logger::getInstance().log(ERROR, "chunk size",
// 						this->_clients[i]);
// 				}
// 				else
// 					Logger::getInstance().log(DEBUG, "chunk part inferior",
// 						this->_clients[i])
// 			}
// 			else
// 				Logger::getInstance().log(DEBUG, "second delim not found",
// 					this->_clients[i]);
// 		}
// 		else
// 			Logger::getInstance().log(DEBUG, "first delim not found",
// 				this->_clients[i]);
// 		return true;	
// 	}
// 	// Logger::getInstance().log(INFO, "not chunked",
// 	// 		this->_clients[i]);//!
// 	return false;
// }


bool Server::isChunked(const size_t i)
{	
	if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
		== "chunked")
	{	
		this->_clients[i].retryChunked = false;
		Logger::getInstance().log(DEBUG, "chunked", this->_clients[i]);
		std::vector<char>::iterator it;
		if (this->_clients[i].chunkSize >= 0
			|| this->isDelimiterFind("\r\n", i, it))
		{
			Logger::getInstance().log(DEBUG, "first delim found",
				this->_clients[i]);
			calculateChunkSize(i, it);
			if (this->isDelimiterFind("\r\n", i, it))
			{	
				Logger::getInstance().log(DEBUG, "second delim found",
					this->_clients[i]);
				stringstream ss; ss << "chunked size "
					<< this->_clients[i].chunkSize;
				Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);

				if (std::distance(this->_clients[i].messageRecv.begin(), it)
					== static_cast<std::ptrdiff_t>(this->_clients[i].chunkSize))
				{
					Logger::getInstance().log(DEBUG, "chunk part completed",
						this->_clients[i]);//!
					std::vector<char> tmp(it, this->_clients[i].messageRecv.end());
					this->_clients[i].messageRecv.erase(it, this->_clients[i].messageRecv.end());
					const size_t tmp_size = this->_clients[i].messageRecv.size();
					this->isBodyEnd(i) ? this->sendBodyEnd(i) :	this->sendBodyPart(i);				
					this->_clients[i].chunkSize -= tmp_size - this->_clients[i].messageRecv.size();
					if (!this->_clients[i].chunkSize)
						this->_clients[i].chunkSize = -1;
									
					int clearNLine = this->_clients[i].messageRecv.empty() ?
						2 : (this->_clients[i].retryChunked = true, 0);
					this->_clients[i].messageRecv.insert(this->_clients[i].messageRecv.end(),
						tmp.begin() + clearNLine, tmp.end()); 
					Server::printVector(this->_clients[i].messageRecv);	
					if (clearNLine == 2 && !this->_clients[i].messageRecv.empty())
						this->_clients[i].retryChunked = true;
				}
				else if (std::distance(this->_clients[i].messageRecv.begin(), it) > static_cast<std::ptrdiff_t>(this->_clients[i].chunkSize))
				{
					Logger::getInstance().log(ERROR, "chunk size",
						this->_clients[i]);
				}
				else
					Logger::getInstance().log(DEBUG, "chunk part inferior",
						this->_clients[i]);
			}
			else
				Logger::getInstance().log(DEBUG, "second delim not found",
					this->_clients[i]);
		}
		else
			Logger::getInstance().log(DEBUG, "first delim not found",
				this->_clients[i]);
		return true;	
	}
	// Logger::getInstance().log(INFO, "not chunked",
	// 		this->_clients[i]);//!
	return false;
}

void Server::calculateChunkSize(const size_t i,
	std::vector<char>::iterator & it)
{
	if (this->_clients[i].chunkSize < 0)
	{
		std::string hexaLen(this->_clients[i].messageRecv.begin(), it);
		std::stringstream ss; ss << hexaLen;
		ssize_t len; ss >> len;		
		if (!ss)
			Logger::getInstance().log(ERROR, "hexadecimal conversion",
				this->_clients[i]);
		else
		{
			stringstream ss; ss << "hexadecimal succes - len: " << len;
			Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]); 
			this->_clients[i].chunkSize = len;
		}
		this->_clients[i].messageRecv.
			erase(this->_clients[i].messageRecv.begin(), it + 2);
	}
}

// bool Server::isChunked(const size_t i)
// {	
// 	if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
// 		== "chunked")
// 	{	
// 		this->_clients[i].retryChunked = false;
// 		Logger::getInstance().log(DEBUG, "chunked", this->_clients[i]);
// 		std::vector<char>::iterator it;
// 		if (this->_clients[i].chunkSize >= 0
// 			|| this->isDelimiterFind("\r\n", i, it))
// 		{
// 			Logger::getInstance().log(DEBUG, "first delim found",
// 				this->_clients[i]);//!		
// 			if 	(this->_clients[i].chunkSize < 0)
// 			{

// 				std::string hexaLen(this->_clients[i].messageRecv.begin(), it);
// 				std::stringstream ss; ss << hexaLen;
// 				ssize_t len;
// 				ss >> len;
// 				if (!ss)
// 					Logger::getInstance().log(ERROR, "hexadecimal conversion",
// 						this->_clients[i]);//!
// 				else
// 				{
// 					stringstream ss; ss << "hexadecimal succes - len: " << len;
// 					Logger::getInstance().log(DEBUG, ss.str(), 
// 						this->_clients[i]);//!
// 					this->_clients[i].chunkSize = len;
// 				}
// 				this->_clients[i].messageRecv.
// 					erase(this->_clients[i].messageRecv.begin(), it + 2);
			
// 			}
// 			if (this->isDelimiterFind("\r\n", i, it))
// 			{	
// 				Logger::getInstance().log(DEBUG, "second delim found",
// 					this->_clients[i]);//!
// 				stringstream ss; ss << "chunked size "
// 					<< this->_clients[i].chunkSize;
// 				Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);//!

// 				if (std::distance(this->_clients[i].messageRecv.begin(), it)
// 					== static_cast<std::ptrdiff_t>(this->_clients[i].chunkSize))
// 				{
// 					Logger::getInstance().log(DEBUG, "chunk part completed",
// 						this->_clients[i]);//!
// 					std::vector<char> tmp(it, this->_clients[i].messageRecv.end());
// 					this->_clients[i].messageRecv.erase(it, this->_clients[i].messageRecv.end());
// 					const size_t tmp_size = this->_clients[i].messageRecv.size();

// 					this->isBodyEnd(i) ? this->sendBodyEnd(i) :	this->sendBodyPart(i);

// 					std::cout << "chunkedsize: 0 " << this->_clients[i].chunkSize << std::endl;
// 					std::cout << "tmp: 0 " << tmp_size  << std::endl;
// 					std::cout << "mess size: 0 " << this->_clients[i].messageRecv.size()  << std::endl;
// 					std::cout << "calcul: 0 " << tmp_size - this->_clients[i].messageRecv.size() << std::endl;
// 					// this->_clients[i].chunkSize = this->_clients[i].messageRecv.empty() ? -1 : tmp_size - this->_clients[i].messageRecv.size();
				
// 					this->_clients[i].chunkSize -= tmp_size - this->_clients[i].messageRecv.size();
// 					if (!this->_clients[i].chunkSize)
// 						this->_clients[i].chunkSize = -1;
// 					std::cout << "chunkedsize: 1 " << this->_clients[i].chunkSize << std::endl;
// 					// int flag = 0;
// 					// if (this->_clients[i].messageRecv.empty())
// 					// {
// 					// 	flag = 2;
// 					// 	// this->_clients[i].retryChunked = true;--
// 					// }
// 					// else
// 					// 	this->_clients[i].retryChunked = true;
// 					int clearNLine = this->_clients[i].messageRecv.empty() ?
// 						2 : (this->_clients[i].retryChunked = true, 0);
// 					this->_clients[i].messageRecv.insert(this->_clients[i].messageRecv.end(),
// 						tmp.begin() + clearNLine, tmp.end()); //! ne marchera pas avec resend	
// 					Server::printVector(this->_clients[i].messageRecv);	
// 					if (clearNLine == 2 && !this->_clients[i].messageRecv.empty())
// 						this->_clients[i].retryChunked = true;

// 				}
// 				else if (std::distance(this->_clients[i].messageRecv.begin(), it) > static_cast<std::ptrdiff_t>(this->_clients[i].chunkSize))
// 				{
// 					Logger::getInstance().log(ERROR, "chunk size",
// 						this->_clients[i]);//!
// 				}
// 				else
// 					Logger::getInstance().log(DEBUG, "chunk part inferior",
// 						this->_clients[i]);//!
// 			}
// 			else
// 				Logger::getInstance().log(DEBUG, "second delim not found",
// 					this->_clients[i]);//!
// 		}
// 		else
// 			Logger::getInstance().log(DEBUG, "first delim not found",
// 				this->_clients[i]);//!
// 		return true;	
// 	}
// 	// Logger::getInstance().log(INFO, "not chunked",
// 	// 		this->_clients[i]);//!
// 	return false;
// }

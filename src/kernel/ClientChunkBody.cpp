#include "Server.hpp"
#include "Logger.hpp"

bool Server::isChunked(const size_t i)
{
	Logger::getInstance().log(DEBUG, "Is Chunked", this->_clients[i]);

	if (this->_clients[i].headerRequest.getHeaders().TransferEncoding
		== "chunked")
	{	
		Logger::getInstance().log(DEBUG, "chunked", this->_clients[i]);

		this->_clients[i].retryChunked = false;
		if (!this->firstDelim(i))	
			Logger::getInstance().log(DEBUG, "first delim not found",
				this->_clients[i]);
		return true;	
	}
	Logger::getInstance().log(INFO, "not chunked", this->_clients[i]);

	return false;
}

bool Server::firstDelim(const size_t i)
{
	std::vector<char>::iterator it;
	if (this->_clients[i].chunkSize >= 0
		|| this->isDelimiterFind("\r\n", i, it))
	{
		Logger::getInstance().log(DEBUG, "first delim found",
			this->_clients[i]);

		this->calculateChunkSize(i, it);
		if (!this->secondDelim(i, it))	
			Logger::getInstance().log(DEBUG, "second delim not found",
				this->_clients[i]);
		return true;
	}
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
		{
			Logger::getInstance().log(ERROR, "hexadecimal conversion",
				this->_clients[i]);
			throw Server::ShortCircuitException(CODE_400_BAD_REQUEST);
		}
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

bool Server::secondDelim(const size_t i, std::vector<char>::iterator & it)
{
	if (this->isDelimiterFind("\r\n", i, it))
	{
		Logger::getInstance().log(DEBUG, "second delim found",
			this->_clients[i]);
		stringstream ss; ss << "chunked size " << this->_clients[i].chunkSize;
		Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);

		if (!this->isChunkPartComplete(i, it))
		{
			if (std::distance(this->_clients[i].messageRecv.begin(), it)
				> static_cast<std::ptrdiff_t>(this->_clients[i].chunkSize))
			{
				Logger::getInstance().log(ERROR, "chunk size",
					this->_clients[i]);
				throw Server::ShortCircuitException(CODE_400_BAD_REQUEST);			
			}			
			else
				Logger::getInstance().log(DEBUG, "chunk part inferior",
					this->_clients[i]);
		}
		return true;
	}
	return false;
}

bool Server::isChunkPartComplete(const size_t i,
	std::vector<char>::iterator & it)
{
	if (std::distance(this->_clients[i].messageRecv.begin(), it)
		== static_cast<std::ptrdiff_t>(this->_clients[i].chunkSize))
	{
		Logger::getInstance().log(DEBUG, "chunk part completed",
			this->_clients[i]);
		std::vector<char> tmp(it, this->_clients[i].messageRecv.end());
		this->_clients[i].messageRecv.
			erase(it, this->_clients[i].messageRecv.end());
		const size_t tmp_size = this->_clients[i].messageRecv.size();
		this->isBodyEnd(i) ? this->sendBodyEnd(i) : this->sendBodyPart(i);
		this->_clients[i].chunkSize
			-= tmp_size - this->_clients[i].messageRecv.size();
		if (!this->_clients[i].chunkSize)
			this->_clients[i].chunkSize = -1;
		int clearNLine = this->_clients[i].messageRecv.empty() ?
			2 : (this->_clients[i].retryChunked = true, 0);
		this->_clients[i].messageRecv.
			insert(this->_clients[i].messageRecv.end(),
			tmp.begin() + clearNLine, tmp.end());
		Server::printVector(this->_clients[i].messageRecv);
		if (clearNLine == 2 && !this->_clients[i].messageRecv.empty())
			this->_clients[i].retryChunked = true;
		return true;
	}
	return false;
}

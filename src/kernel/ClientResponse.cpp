#include "Server.hpp"
#include "Logger.hpp"

void Server::replyClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{	
		try 
		{
			if (this->_clients[i].ping)
				continue;
			if (this->_clients[i].sendFlag)
			{
				if (this->replyClient(i, this->_clients[i].sendBuffer))
					break ;
			}		
			else if (!this->_clients[i].headerRespons.empty())		
				this->sendBuffering(i, this->_clients[i].headerRespons);
			else if (this->_clients[i].messageSend.empty())		
				this->fillMessageSend(i);
			else 
				this->sendBuffering(i, this->_clients[i].messageSend);	
		}
		catch (const Server::ShortCircuitException & e)	
		{	this->shortCircuit(e.getCode(), i);	}
	}
}

void Server::fillMessageSend(const size_t i)
{	
	if (this->_clients[i].responseBuilder.getBody(this->_clients[i]))
		return ;
	if (this->_clients[i].messageSend.empty())
		this->_clients[i].sendFlag = true;
	else										
		this->sendBuffering(i, this->_clients[i].messageSend);
}

void Server::sendBuffering(const size_t i, std::vector<char> & response)
{
	Logger::getInstance().log(DEBUG, "Send Buffering", this->_clients[i]);
	 
	this->_clients[i].sendBuffer.insert(this->_clients[i].sendBuffer.end(),
		response.begin(), response.end());
	response.clear();
}

bool Server::replyClient(const size_t i, std::vector<char> & resp)
{	
	Logger::getInstance().log(INFO, "Reply Client", this->_clients[i]);
	Server::printVector(this->_clients[i], resp);
	if (!FD_ISSET(this->_clients[i].fd, &Kernel::_writeSet))
		return Logger::getInstance().log(DEBUG, "not ready to reply",
			this->_clients[i]), false;
	Logger::getInstance().log(DEBUG, "\e[31;103mready to reply\e[0m",
		this->_clients[i]);
	ssize_t ret = send(this->_clients[i].fd, resp.data(), std::min(resp.size(),
		static_cast<size_t>(this->_conf.send_buff_size)), MSG_NOSIGNAL);
	Kernel::cleanFdSet(this->_clients[i]);	
	if (ret <= 0)
	{
		if (ret < 0)
			Logger::getInstance().log(ERROR, "send", this->_clients[i]);
		this->exitClient(i);
		return true;
	}
	std::vector<char> str(resp.data(), resp.data() + static_cast<size_t>(ret));  		
	Logger::getInstance().log(DEBUG, "sent to client: ", this->_clients[i]); 
	Server::printVector(this->_clients[i], str, RED, static_cast<int>(INFO));
	resp.erase(resp.begin(), resp.begin() + ret);
	if (resp.empty())
		return this->endReply(i);
	return false;
}

bool Server::endReply(const size_t i)
{
	Logger::getInstance().log(DEBUG, "End Reply", this->_clients[i]);

	if (this->_clients[i].exitRequired)	
		return this->exitClient(i), true;						
	this->_clients[i].headerRequest = RequestParser();
	this->_clients[i].responseBuilder = ResponseBuilder();
	this->_clients[i].chunkSize = -1;
	this->_clients[i].bodySize = 0;
	this->_clients[i].ping = true;		
	this->_clients[i].messageSend.clear();
	this->_clients[i].sendFlag = false;
	return false;
}

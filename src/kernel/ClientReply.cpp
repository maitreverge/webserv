#include "Server.hpp"
#include "Logger.hpp"

void Server::replyClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{		
		if (this->_clients[i].ping)
			continue;	
		if (!this->_clients[i].headerRespons.empty())
		{	
			if (replyClient(i, this->_clients[i].headerRespons))
				break ;	
		}
		else if (this->_clients[i].messageSend.empty())
		{
			if (fillMessageSend(i))
				break ;
		}
		else if (replyClient(i, this->_clients[i].messageSend))
			break ;
	}
}

bool Server::fillMessageSend(size_t i)
{
	if (this->_clients[i].responseBuilder.getBody(this->_clients[i]))
		return false;
	if (!this->_clients[i].messageSend.empty())									
		return replyClient(i, this->_clients[i].messageSend);	
	else
		return endReply(i);	
}

bool Server::endReply(size_t i)
{
	Logger::getInstance().log(DEBUG, "End Reply",
		this->_clients[i]);

	if (this->_clients[i].exitRequired)	
		return this->exitClient(i), true;						
	this->_clients[i].headerRequest = RequestParser();
	this->_clients[i].responseBuilder = ResponseBuilder();
	this->_clients[i].chunkedSize = -1;
	this->_clients[i].bodySize = 0;
	this->_clients[i].ping = true;		
	this->_clients[i].messageSend.clear();
	return false;
}

void Server::printResponse(const std::vector<char> & response)
{
	std::cout << std::endl << "\e[34mPrint Vector: \e[31m" << std::endl;
	std::cout << "-";		
	for (size_t i = 0; i < response.size(); i++)				
		std::cout << response[i];
	std::cout << "-\e[0m" << std::endl << std::endl;
}

bool Server::replyClient(size_t i, std::vector<char> & response)
{	
	Logger::getInstance().log(INFO, "Reply Client", this->_clients[i]);
	this->printResponse(response);
	if (!FD_ISSET(this->_clients[i].fd, &Kernel::_writeSet))
		return Logger::getInstance().log(DEBUG, "not ready to reply Client",
			this->_clients[i]), false;
	Logger::getInstance().log(WARNING, "ready to reply Client",
		this->_clients[i]);
	ssize_t ret = send(this->_clients[i].fd, response.data(), response.size(),
		MSG_NOSIGNAL);
	Kernel::cleanFdSet(this->_clients[i]);	
	if (ret <= 0)
	{
		if (ret < 0)
			Logger::getInstance().log(ERROR, "send", this->_clients[i]);
		this->exitClient(i);
		return true;
	}
	std::string str(response.data(), response.data()
		+ static_cast<size_t>(ret));      
	std::stringstream ss; ss << "data sent to client: -" << str << "-";	
	Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]); 

	response.erase(response.begin(), response.begin() + ret);
	return false;
}

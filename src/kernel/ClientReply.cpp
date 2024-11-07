#include "Server.hpp"
#include "Logger.hpp"

void Server::replyClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{		
		if (this->_clients[i].ping < 2 
			|| !FD_ISSET(this->_clients[i].fd, &this->_writeSet))
				continue;	
		if (!this->_clients[i].headerRespons.empty())			
			if (replyClient(i, this->_clients[i].headerRespons,
				static_cast<ssize_t>
				(this->_clients[i].headerRespons.size())))
				break ;	
		if (this->_clients[i].messageSend.empty())
		{
			if (fillMessageSend(i))
				break ;
		}
		else if (replyClient(i, this->_clients[i].messageSend,
			static_cast<ssize_t>(this->_clients[i].messageSend.size())))
			break ;
	}
}

bool Server::fillMessageSend(size_t i)
{
	if (ssize_t ret = this->_clients[i].responseBuilder.
		getBody(this->_clients[i]))
	{
		if (ret == 73)
			return false;							
		if (replyClient(i, this->_clients[i].messageSend, ret))
			return true;								
		usleep(500);
	}
	else if (endReply(i))
		return true;
	return false;
}

bool Server::endReply(size_t i)
{
	Logger::getInstance().log(DEBUG, "reinit response Builder",
		this->_clients[i]);
	if (this->_clients[i].exitRequired)	
		return this->exitClient(i), true;						
	this->_clients[i].headerRequest = RequestParser();
	this->_clients[i].responseBuilder = ResponseBuilder();
	this->_clients[i].bodySize = 0;
	this->_clients[i].ping = 0;		
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

bool Server::replyClient(size_t i, std::vector<char> & response,
	ssize_t repSize)
{	
	Logger::getInstance().log(INFO, "reply client", this->_clients[i]);
	printResponse(response);

	response.erase(response.begin() + repSize, response.end());
	ssize_t ret;
		
	if ((ret = send(this->_clients[i].fd, response.data(), response.size(),
		MSG_NOSIGNAL)) < 0)
	return Logger::getInstance().log(ERROR, "send", this->_clients[i]),
		this->exitClient(i), true;		

	std::string str(response.data(), response.data()
		+ static_cast<size_t>(ret));      
	std::stringstream ss; ss << "data sent to client: -" << str << "-";	
	Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]); 

	response.erase(response.begin(), response.begin() + ret);	
	return false;
}

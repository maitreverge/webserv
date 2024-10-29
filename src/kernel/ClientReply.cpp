#include "Server.hpp"
#include "Logger.hpp"

void Server::replyClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{		
		if (!this->_clients[i].ping
			&& FD_ISSET(this->_clients[i].fd, &this->_writeSet))
		{
			if (!this->_clients[i].pongHeader)
			{				
				if (replyClient(i, this->_clients[i].headerRespons,
					static_cast<ssize_t>
					(this->_clients[i].headerRespons.size())))
					break ;				
				this->_clients[i].pongHeader = true;				
			}				
			if (ssize_t ret = this->_clients[i].responseBuilder.
				getBody(this->_clients[i]))
			{							
				if (replyClient(i, this->_clients[i].messageSend, ret))
					break ;								
				usleep(5000);
			}
			else
			{
				Logger::getInstance().log(DEBUG, "reinit response Builder",
					this->_clients[i]);

				this->_clients[i].responseBuilder = ResponseBuilder();
				this->_clients[i].ping = true;			
				this->_clients[i].pongHeader = false;				
			}
		}	
	}
}

void printVectorCharTest(const std::vector<char> & vect) //!supp
{
	std::cout << std::endl << "\e[34mPrint Vector: \e[31m" << std::endl;
	std::cout << "-";		
	for (size_t i = 0; i < vect.size(); i++)				
		std::cout << vect[i];
	std::cout << "-\e[0m" << std::endl << std::endl;
}

void printMessageClientTest(const Client & client) //!supp
{
	std::cout << std::endl << "\e[34mmessage client: " << std::endl;		
	for (size_t i = 0; i < client.messageRecv.size(); i++)				
		std::cout << client.messageRecv[i];
	std::cout << "\e[0m" << std::endl << std::endl;
}

bool Server::replyClient(size_t i, std::vector<char> & response,
	ssize_t repSize)
{	
	Logger::getInstance().log(INFO, "reply client", this->_clients[i]);
	printVectorCharTest(response);

	this->_writeBuffer.assign(response.begin(), response.begin() + repSize);					

	ssize_t ret;
	char * writeHead = this->_writeBuffer.data();
	size_t writeSize = this->_writeBuffer.size();
	while (writeSize > 0)
	{			
		if ((ret = send(this->_clients[i].fd, writeHead, writeSize,
			MSG_NOSIGNAL)) < 0)
		{
			Logger::getInstance().log(ERROR, "send", this->_clients[i]);
			this->exitClient(i);
			return true;
		}		

		std::string str(writeHead, writeHead + static_cast<size_t>(ret));
		std::stringstream ss; ss << "data sent to client: -" << str << "-";	
		Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]); 	

		writeHead += ret;
		writeSize -= static_cast<size_t>(ret);
	}
	return false;
}

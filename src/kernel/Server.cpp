#include "Server.hpp"
#include "Logger.hpp" 
#include "ConfigFileParser.hpp"

Server::Server(sockaddr_in & sockAddr, Config * conf)
	: _sockAddr(sockAddr), _conf(conf)
{		
	static int i;	
	this->_conf->index = i++;
	this->_clients.reserve(static_cast<size_t>(this->_conf->maxClient));
	this->_readBuffer.reserve(RECV_BUFF_SIZE);	
	this->_writeBuffer.reserve(SEND_BUFF_SIZE);
}

const sockaddr_in & Server::getSockAdress() const
{
	return (this->_sockAddr);
}

bool Server::setup()
{	
	this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (this->_fd < 0)	
		return Logger::getInstance().log(ERROR, "socket"), false;
	Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fd);
	FD_SET(this->_fd, &Kernel::_actualSet);
	if (bind(this->_fd, reinterpret_cast<const sockaddr *>
		(&this->_sockAddr), sizeof(this->_sockAddr)) < 0)
	{		
		Logger::getInstance().log(ERROR, "bind", *this);

		this->exitServer();		
		return false;		
	}	
	if (listen(this->_fd, this->_conf->maxClient) < 0)
	{
		Logger::getInstance().log(ERROR, "listen", *this);

		this->exitServer();
		return false;
	}
	Logger::getInstance().log(INFO, "listen", *this);
	return true;
}

void Server::catchClients(Kernel & kernel)
{
	if (FD_ISSET(this->_fd, &Kernel::_readSet))
	{	
		Client client(&kernel._conf);			
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>
			(&client.address), &client.addressLen);			
		if (client.fd < 0)		
			return Logger::getInstance().log(ERROR, "accept");		
		if (kernel.countClients() >= this->_conf->maxClient)		
			return close(client.fd),
				Logger::getInstance().log(ERROR, "max client reached", client);					
		Logger::getInstance().log(INFO, "\e[30;101mnew client\e[0m", client);
		struct timeval timeout = {SND_TIMEOUT, 0};	
		if (setsockopt(client.fd, SOL_SOCKET, SO_SNDTIMEO, &timeout,
			sizeof(timeout)) < 0)
	  		return close(client.fd),
				Logger::getInstance().log(ERROR, "send timeout", client);
		FD_SET(client.fd, &Kernel::_actualSet);
		Kernel::_maxFd = std::max(Kernel::_maxFd, client.fd);
		this->_clients.push_back(client);				
	}
}

void Server::displayClient(Client & client) const
{
	std::stringstream ss;
	ss << "new client" << " - Fd: " << client.fd << " Family: "
		<< client.address.sin_family << " Addres: "
		<< inet_ntoa(client.address.sin_addr) << " Port: "
		<< ntohs(client.address.sin_port);
	Logger::getInstance().log(INFO, ss.str(), client);
}

void Server::exitClient(size_t i)
{
	Logger::getInstance().log(INFO, "\e[30;101mclient exited\e[0m",
		this->_clients[i]);

	FD_CLR(this->_clients[i].fd, &Kernel::_actualSet);
	close(this->_clients[i].fd);	
	this->_clients.erase(this->_clients.begin() + static_cast<int>(i));
}

void Server::exitClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		FD_CLR(this->_clients[i].fd, &Kernel::_actualSet);
		close(this->_clients[i].fd);	
	}
	this->_clients.clear();
}

void Server::exitServer()
{
	this->exitClients();
	FD_CLR(this->_fd, &Kernel::_actualSet);
	close(this->_fd);	
}

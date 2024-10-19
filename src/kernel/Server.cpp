#include <Server.hpp>

Server::Server(sockaddr_in & sockAddr, int & maxFd,
fd_set & actualSet, fd_set & readSet, fd_set & writeSet)
	: _sockAddr(sockAddr), _maxFd(maxFd),
	_actualSet(actualSet), _readSet(readSet), _writeSet(writeSet) 
{	
	this->_readBuffer.resize(BUFF_SIZE);
	this->_writeBuffer.resize(BUFF_SIZE);
}

bool Server::setup()
{	
	this->_maxFd = this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)	
		return std::cout << "error socket" << std::endl, false;
	FD_SET(this->_fd, &_actualSet);
	if (bind(this->_fd, reinterpret_cast<const sockaddr *>
		(&this->_sockAddr), sizeof(this->_sockAddr)) < 0)	
		return std::cout << "error bind from socket: " <<
			ntohs(this->_sockAddr.sin_port) << std::endl, false;		
	if (listen(this->_fd, this->_conf.maxClient) < 0)	
		return std::cout << "error listen" << std::endl, false;
	return true;
}

void Server::catchClients()
{	
	if (FD_ISSET(this->_fd, &this->_readSet))
	{		
		Client client;			
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>
			(&client.address), &client.len);
		if (client.fd < 0)
		{
			std::cout << "error accept" << std::endl;		
			return ;
		}
		displayClient(client);
		FD_SET(client.fd, &this->_actualSet);
		this->_maxFd = std::max(this->_maxFd, client.fd);
		this->_clients.push_back(client);
		welcomeClient(client);			
	}
}

void Server::listenClients()
{	
	for (size_t i = 0; i < this->_clients.size(); i++)
	{	
		if (FD_ISSET(this->_clients[i].fd, &this->_readSet))
		{
			ssize_t ret = recv(this->_clients[i].fd, this->_readBuffer.data(),
				this->_readBuffer.size(), 0);
			if (ret < 0)
			{
				std::cout << "error recev" << std::endl;
				exitClient(i);				
			}
			else if (ret == 0)					
				exitClient(i);		
			else
			{
				// std::search(this->_read"\r\n\")
				if (this->_readBuffer.size() + this->_clients[i].message.size() > MAX_HDR_SIZE)
					std::cout << "error header size" << std::endl;
				this->_clients[i].message.insert(this->_clients[i].message.end(),
					this->_readBuffer.begin(), this->_readBuffer.end());
				if (std::search(this->_clients[i].message.end() - 4,
					this->_clients[i].message.end(), "\r\n\r\n",
					"\r\n\r\n" + 4) != this->_clients[i].message.end())
				{
					this->_parser.parse(this->_clients[i].message);								
					this->_parser.displayAttributes();	
					this->_clients[i].message.clear();
				}

				for (size_t j = 0; j < this->_readBuffer.size(); j++)//!
					std::cout << "client say: " << ret << std::endl;
				this->_readBuffer.resize(static_cast<size_t>(ret));
				for (size_t i = 0; i < this->_readBuffer.size(); i++)				
					std::cout << this->_readBuffer[i];
				std::cout << std::endl;		
				
					
			}
		}	
	}
}

void Server::displayClient(Client & client)
{
	std::cout << "new client:" << std::endl;
	std::cout << "fd: " << client.fd << std::endl;
	std::cout << "family: " << client.address.sin_family << std::endl;
	std::cout << "addres: " << inet_ntoa(client.address.sin_addr) << std::endl;
	std::cout << "port: " << ntohs(client.address.sin_port) << std::endl;
	for (int i = 0; i < 8; i++)
		std::cout << "sin zero: " << ntohl(client.address.sin_zero[i])
			<< std::endl;
}

void Server::welcomeClient(Client & client)
{
	std::string welcome = "welcome Bitch!\n";
	this->_writeBuffer.assign(welcome.begin(), welcome.end());			
	this->_readSet = this->_writeSet = this->_actualSet;		
	if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL)
		< 0)
		std::cout << "error select" << std::endl;
	if(FD_ISSET(client.fd, &this->_writeSet))
	{
		ssize_t ret = send(client.fd, this->_writeBuffer.data(),
			this->_writeBuffer.size(), 0);
		if (ret < 0)
		{
			std::cout << "error send" << std::endl;
			return ;
		}
		else if (ret != static_cast<ssize_t>(this->_writeBuffer.size()))		
			std::cout << "warning not all data sent" << std::endl;		
	}
}

void Server::exitClient(size_t i)
{
	std::cout << "client exited" << std::endl;	
	FD_CLR(this->_clients[i].fd, &this->_actualSet);
	close(this->_clients[i].fd);
	this->_clients[i].fd = -1;
	this->_clients.erase(this->_clients.begin() + i);
}

void Server::exitClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].fd != -1)
			close(this->_clients[i].fd);
	}
}

void Server::exitServer()
{
	exitClients();
	close(this->_fd);	
}
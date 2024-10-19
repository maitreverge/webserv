#include <Server.hpp>

Server::Server(sockaddr_in & sockAddr, int & maxFd,
fd_set & actualSet, fd_set & readSet, fd_set & writeSet)
	: _sockAddr(sockAddr), _maxFd(maxFd),
	_actualSet(actualSet), _readSet(readSet), _writeSet(writeSet) 
{	
	this->_readBuffer.resize(BUFF_SIZE);
	this->_maxFd = this->_fd = socket(AF_INET, SOCK_STREAM, 0);	
	FD_SET(this->_fd, &_actualSet);
	if (bind(this->_fd, reinterpret_cast<const sockaddr *>
		(&this->_sockAddr), sizeof(this->_sockAddr)) < 0)
		std::cout << "error bind from socket: " <<
			ntohs(this->_sockAddr.sin_port) << std::endl;
	if (listen(this->_fd, this->_conf.maxClient) < 0)
		std::cout << "error listen" << std::endl;
}

void Server::catchClients()
{	
	if (FD_ISSET(this->_fd, &this->_readSet))
	{		
		Client client;			
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>
			(&client.address), &client.len);
		if (client.fd < 0)
			std::cout << "error client" << std::endl;		
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
				std::cout << "error recev" << std::endl;
			if (ret == 0)					
				exitClient(i);		
			else
			{
				std::cout << "client say: " << ret << std::endl;
				this->_readBuffer.resize(static_cast<size_t>(ret));
				for (size_t i = 0; i < this->_readBuffer.size(); i++)				
					std::cout << this->_readBuffer[i];
				std::cout << std::endl;		
				
				this->_parser.parse(this->_readBuffer);
				this->_readBuffer.resize(BUFF_SIZE);				
				this->_parser.displayAttributes();		
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
	if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL) < 0)
		std::cout << "error select" << std::endl;
	if(FD_ISSET(client.fd, &this->_writeSet))
		send(client.fd, this->_writeBuffer.data(), this->_writeBuffer.size(), 0);
}

void Server::exitClient(size_t i)
{
	std::cout << "client quit" << std::endl;
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
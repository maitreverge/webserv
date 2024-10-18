#include <Server.hpp>


Server::Server(sockaddr_in sockAddr, int & maxFd, fd_set & actualSet, fd_set & readSet, fd_set & writeSet)
: _sockAddr(sockAddr), _maxFd(maxFd), _actualSet(actualSet), _readSet(readSet), _writeSet(writeSet) 
{
	this->_readBuffer.resize(300);
	this->_maxFd = this->_fd = socket(AF_INET, SOCK_STREAM, 0);	
	FD_SET(this->_fd, &_actualSet);	
	bind(this->_fd, reinterpret_cast<const sockaddr *>
		(&this->_sockAddr), sizeof(this->_sockAddr));
	listen(this->_fd, this->_conf.maxClient);	
}

void Server::catchClient()
{	
	if (FD_ISSET(this->_fd, &this->_readSet))
	{		
		Client client;
		client.id = 1;
		socklen_t len;
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>(&client.address), &len);
		if (client.fd < 0)
			std::cout << "error client" << std::endl;
		FD_SET(client.fd, &this->_actualSet);
		this->_maxFd = std::max(this->_maxFd, client.fd);
		this->_clients.push_back(client);					
		std::string welcome = "welcome Bitch!\n";
		this->_writeBuffer.assign(welcome.begin(), welcome.end());			
		this->_readSet = this->_writeSet = this->_actualSet;		
		if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL) < 0)
			std::cout << "error select" << std::endl;
		if(FD_ISSET(client.fd, &_writeSet))
			send(client.fd, this->_writeBuffer.data(), this->_writeBuffer.size(), 0);		
	}
}

void Server::listenClient()
{	
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		// std::cout << "listen" << std::endl;
		if (FD_ISSET(this->_clients[i].fd, &this->_readSet))
		{
			ssize_t ret = recv(this->_clients[i].fd, this->_readBuffer.data(), this->_readBuffer.size(), 0);
			if (ret <= 0)
			{
				std::cout << "client quit" << ret  << std::endl;
				FD_CLR(this->_clients[i].fd, &this->_actualSet);
				close(this->_clients[i].fd);
				this->_clients[i].fd = -1;
				// del client ? 
			}
			else
			{
				std::cout << "client say: " << ret << std::endl;
				this->_readBuffer.resize(static_cast<size_t>(ret));
				for (size_t i = 0; i < this->_readBuffer.size(); i++)				
					std::cout << this->_readBuffer[i];
				std::cout << std::endl;				
			}
		}	
	}
}
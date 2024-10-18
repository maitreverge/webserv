#include <Server.hpp>


Server::Server(sockaddr_in sockAddr, int & maxFd, fd_set & actualSet, fd_set & readSet, fd_set & writeSet)
: _sockAddr(sockAddr), _maxFd(maxFd), _actualSet(actualSet), _readSet(readSet), _writeSet(writeSet) 
{
	std::cout << "startclient setup0" << std::endl;
	this->_maxFd = this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	FD_SET(this->_fd, &_actualSet);	
	bind(this->_fd, reinterpret_cast<const sockaddr *>
		(&this->_sockAddr), sizeof(this->_sockAddr));
	listen(this->_fd, this->_conf.maxClient);
	std::cout << "startclient setup" << std::endl;
}

void Server::catchClient()
{	std::cout << "startclient" << std::endl;
	if (FD_ISSET(this->_fd, &this->_readSet))
	{	
		std::cout << "startclient2" << std::endl;
		Client client;
		client.id = 1;
		socklen_t len;
		int clientFd = accept(this->_fd, reinterpret_cast<sockaddr *>(&client.address), &len);
		if (clientFd < 0)
			std::cout << "error client" << std::endl;
	std::cout << "startclient3" << std::endl;
		FD_SET(clientFd, &this->_actualSet);
		this->_maxFd = std::max(this->_maxFd, clientFd);
		this->_clients.push_back(client);
					
		std::string welcome = "welcome Bitch!";
		this->_writeBuffer.assign(welcome.begin(), welcome.end());			
		std::cout << "hey" << std::endl;
		this->_readSet = this->_writeSet = this->_actualSet;
		// sleep(1);		
		if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL) < 0)
			std::cout << "error select" << std::endl;
		std::cout << "hey2" << std::endl;
		if(FD_ISSET(clientFd, &_writeSet))
			send(clientFd, this->_writeBuffer.data(), this->_writeBuffer.size(), 0);		
	}
}
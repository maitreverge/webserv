#include <Server.hpp>


Server::Server(int & maxFd, fd_set & actualSet, fd_set & readSet, fd_set & writeSet)
: _maxFd(maxFd), _actualSet(actualSet), _readSet(readSet), _writeSet(writeSet) 
{
	this->_maxFd = this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	bind(this->_fd, reinterpret_cast<const sockaddr *>
		(&this->_conf.sockAddress.at(0)), sizeof(_conf.sockAddress.at(0)));
	FD_ZERO(&_actualSet);
	FD_SET(this->_fd, &_actualSet);
	listen(this->_fd, this->_conf.maxClient);
}

void Server::catchClient()
{
	if (FD_ISSET(this->_fd, &this->_readSet))
	{	
		Client client;
		client.id = 1;
		socklen_t len;
		int clientFd = accept(this->_fd, reinterpret_cast<sockaddr *>(&client.address), &len);
		if (clientFd < 0)
			std::cout << "error client" << std::endl;
	
		FD_SET(clientFd, &this->_actualSet);
		this->_maxFd = std::max(this->_maxFd, clientFd);
		this->_clients.push_back(client);
					
		std::string welcome = "welcome Bitch!";
		this->_write_buffer.assign(welcome.begin(), welcome.end());			
	
		this->_readSet = this->_writeSet = this->_actualSet;		
		if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL) < 0)
			std::cout << "error select" << std::endl;
		if(FD_ISSET(clientFd, &_writeSet))
			send(clientFd, this->_write_buffer.data(), this->_write_buffer.size(), 0);		
	}
}
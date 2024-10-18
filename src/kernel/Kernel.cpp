#include "Kernel.hpp"

Kernel::Kernel(void)
{
	std::cout << "hello from kernel" << std::endl;


}
Kernel::~Kernel(void)
{

}
Kernel::Kernel(const Kernel &)
{

}

Kernel & Kernel::operator=(const Kernel &)
{
	return *this;
}

// void Kernel::catchClient(int serverFd)
// {
// 	if (FD_ISSET(serverFd, &this->_readSet))
// 	{	
// 		Client client;
// 		client.id = 1;
// 		socklen_t len;
// 		int clientFd = accept(serverFd, reinterpret_cast<sockaddr *>(&client.address), &len);
// 		if (clientFd < 0)
// 			std::cout << "error client" << std::endl;
	
// 		FD_SET(clientFd, &this->_actualSet);
// 		this->_maxFd = std::max(this->_maxFd, clientFd);
// 		this->_clients.push_back(client);
					
// 		std::string welcome = "welcome Bitch!";
// 		this->_write_buffer.assign(welcome.begin(), welcome.end());			
	
// 		this->_readSet = this->_writeSet = this->_actualSet;		
// 		if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL) < 0)
// 			std::cout << "error select" << std::endl;
// 		if(FD_ISSET(clientFd, &_writeSet))
// 			send(clientFd, this->_write_buffer.data(), this->_write_buffer.size(), 0);		
// 	}
// }
void Kernel::setup()
{
	// int serverFd = socket(AF_INET, SOCK_STREAM, 0);
	// int maxFd = serverFd;
	// bind(serverFd, reinterpret_cast<const sockaddr *>(&this->_conf.sockAddress.at(0)), sizeof(_conf.sockAddress.at(0)));
	// FD_ZERO(&_actualSet);
	// FD_SET(serverFd, &_actualSet);
	// listen(serverFd, this->_conf.maxClient);
	
	Server server1(this->_maxFd, this->_actualSet, this->_readSet, this->_writeSet);
	this->_servers.push_back(server1);

	while (true)
	{
		this->_readSet = this->_writeSet = this->_actualSet;

		if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL) < 0)
		{	
			std::cout << "error select" << std::endl;
			continue;
		}
		this->_servers.back().catchClient();
		// catchClient(serverFd);
		// if (FD_ISSET(serverFd, &this->_readSet))
		// {	
		// 	Client client;
		// 	client.id = 1;
		// 	socklen_t len;
		// 	int clientFd = accept(serverFd, reinterpret_cast<sockaddr *>(&client.address), &len);
		// 	if (clientFd < 0)
		// 		std::cout << "error client" << std::endl;
		
		// 	FD_SET(clientFd, &_actualSet);
		// 	maxFd = std::max(maxFd, clientFd);
		// 	this->clients.push_back(client);
						
		// 	std::string welcome = "welcome Bitch!";
		// 	this->_write_buffer.assign(welcome.begin(), welcome.end());			
		
		// 	this->_readSet = this->_writeSet = this->_actualSet;		
		// 	if (select(maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL) < 0)
		// 		std::cout << "error select" << std::endl;
		// 	if(FD_ISSET(clientFd, &_writeSet))
		// 		send(clientFd, this->_write_buffer.data(), this->_write_buffer.size(), 0);		
		// }
		
	}
}
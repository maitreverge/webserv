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
#include <unistd.h>
void Kernel::setup()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	int maxFd = fd;
	bind(fd, reinterpret_cast<const sockaddr *>(&this->conf.sockAddress.at(0)), sizeof(conf.sockAddress.at(0)));
	FD_ZERO(&actualSet);
	FD_SET(fd, &actualSet);
	listen(fd, this->conf.max_client);
		std::cout << "bloom 000" << std::endl;

		struct timeval timeout;
	timeout.tv_sec = 0;  // 5 seconds
	timeout.tv_usec = 0;
	while (true)
	{
		this->readSet = this->writeSet = this->actualSet;
			// std::cout << "bloom " << maxFd << std::endl;
		if (select(maxFd + 1, &this->readSet, &this->writeSet, 0, &timeout) < 0)
			std::cout << "error select" << std::endl;

		// std::cout << "bloom2" << std::endl;
		if (FD_ISSET(fd, &this->readSet))
		{
				std::cout << "bloom2" << std::endl;
			Client client;
			client.id = 1;
			socklen_t len;
			int clientFd = accept(fd, reinterpret_cast<sockaddr *>(&client.address), &len);
			if (clientFd < 0)
				std::cout << "error client" << std::endl;
			// int clientFd = accept(fd, reinterpret_cast<sockaddr *>(&this->conf.sockAddress.at(0)), &len);
			FD_SET(clientFd, &actualSet);
			maxFd = std::max(maxFd, clientFd);
			this->clients.push_back(client);
			std::cout << "before im ready" << std::endl;
		
			std::cout << "im ready" << std::endl;			
			std::string welcome = "welcome Bitch!";
			this->write_buffer.assign(welcome.begin(), welcome.end());
			for (size_t i = 0; i < write_buffer.size(); i++)
				std::cout << this->write_buffer[i] << std::endl;
			// sleep(2);
				this->readSet = this->writeSet = this->actualSet;
			// std::cout << "bloom " << maxFd << std::endl;
			if (select(maxFd + 1, &this->readSet, &this->writeSet, 0, &timeout) < 0)
				std::cout << "error select" << std::endl;
			if(FD_ISSET(clientFd, &writeSet))	
			{
				std::cout << "IS WRITE" << std::endl;

				send(clientFd, this->write_buffer.data(), this->write_buffer.size(), 0);		
			}
			std::cout << "end" << std::endl;	
		}
		
	}
}
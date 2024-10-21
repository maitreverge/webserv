#include "Kernel.hpp"

bool Kernel::_exit = false;

Kernel::Kernel(void)
{
	Logger::getInstance().log("hello from kernel", *this);

	this->setup();
	this->launch();
}

void Kernel::callCatch(Server & server)
{
	server.catchClients();
}

void Kernel::callListen(Server & server)
{
	server.listenClients();
}

void Kernel::callExit(Server & server)
{
	server.exitServer();
}

void Kernel::setup()
{
	FD_ZERO(&this->_actualSet);
	for (size_t i = 0; i < this->_conf.sockAddress.size(); i++)
	{
		Server server(this->_conf.sockAddress[i], this->_maxFd,
			this->_actualSet, this->_readSet, this->_writeSet);
		if (server.setup())
			this->_servers.push_back(server);	
	}	
}

void Kernel::launch()
{
	while (true)
	{
		struct timeval timeout = {1, 0};
		this->_readSet = this->_writeSet = this->_actualSet;

		if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet,
			0, &timeout) < 0)
		{	
			if (!this->_exit)
				Logger::getInstance().log("error select");
			continue;
		}
		if (this->_exit)
			break;
		std::for_each(this->_servers.begin(), this->_servers.end(),
			this->callCatch);
		std::for_each(this->_servers.begin(), this->_servers.end(),
			this->callListen);		
	}

	std::for_each(this->_servers.begin(), this->_servers.end(),
		this->callExit);
}
#include "Kernel.hpp"

// Kernel::_exit = false;
Kernel::Kernel(void)
{
	std::cout << "hello from kernel" << std::endl;
	this->setup();
	this->launch();
}

void Kernel::signalHandle(int)
{
	std::cout << "Bye bye!" << std::endl;
	Kernel::_exit = true;
}

void Kernel::disableSignalEcho()
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);     
    tty.c_lflag &= ~ECHOCTL;  
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); 
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
	server.exitClients();
}

void Kernel::setup()
{	
	signal(2, this->signalHandle);
	disableSignalEcho();
	FD_ZERO(&this->_actualSet);
	for (size_t i = 0; i < this->_conf.sockAddress.size(); i++)
	{
		Server server(this->_conf.sockAddress[i], this->_maxFd,
			this->_actualSet, this->_readSet, this->_writeSet);
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
			std::cout << "error select" << std::endl;
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
#include "Kernel.hpp"

bool Kernel::_exit = false;
int Kernel::_maxFd = 0;
fd_set Kernel::_actualSet;
fd_set Kernel::_readSet;
fd_set Kernel::_writeSet;

Kernel::Kernel(void) : _conf()
{
	this->_servers.reserve(this->_conf.sockAddress.size());
	FD_ZERO(&this->_actualSet);
	this->setup();	
}

Kernel::Kernel(char* path) : _conf(path)
{
	this->_servers.reserve(this->_conf.sockAddress.size());
	FD_ZERO(&this->_actualSet);
	this->setup();
}

Kernel::~Kernel()
{
	for (size_t i = 0; i < this->_servers.size(); i++)
		this->_servers[i].exitServer();
}

Kernel & Kernel::getInstance(char *path)
{
	static Kernel * kernel_ptr;
	if (!path)
	{
		if (kernel_ptr)	
			return *kernel_ptr;			
		static Kernel kernel;
		kernel_ptr = &kernel;
		kernel.launch();		
		return kernel;
	}	
	static Kernel kernel(path);
	kernel_ptr = &kernel;
	kernel.launch();
	return kernel;	
}

void Kernel::callCatch()
{
	for (size_t i = 0; i < this->_servers.size(); i++)
		this->_servers[i].catchClients(*this);
}

void Kernel::callListen(Server & server)
{
	server.listenClients();
}

void Kernel::callReply(Server & server)
{
	server.replyClients();
}

short int Kernel::countClients()
{
	short int nClients = 0;

	for (size_t i = 0; i < this->_servers.size(); i++)
		nClients += static_cast<short int>(this->_servers[i]._clients.size());
	return nClients;
}

void Kernel::setup()
{	
	for (size_t i = 0; i < this->_conf.sockAddress.size(); i++)
	{
		Server server(this->_conf.sockAddress[i], this->_conf);
		if (server.setup())
			this->_servers.push_back(server);	
	}
	std::cout << std::endl;		
}

void Kernel::cleanFdSet(Client & client)
{
	if (client.fd > 0)
	{
		FD_CLR(client.fd, &Kernel::_writeSet);
		FD_CLR(client.fd, &Kernel::_readSet);
	}
	if (client.responseBuilder._cgi._fds[1] > 0)
	{
		FD_CLR(client.responseBuilder._cgi._fds[1], &Kernel::_writeSet);
		FD_CLR(client.responseBuilder._cgi._fds[1], &Kernel::_readSet);	
	}
}

void Kernel::launch()
{
	while (true)
	{
		struct timeval timeout = {SLCT_TIMEOUT, 0};
		Kernel::_readSet = Kernel::_writeSet = Kernel::_actualSet;	
		// sleep(1);
		// Logger::getInstance().log(WARNING, "\e[31;43mSELECT\e[0m");	
		if (select(Kernel::_maxFd + 1, &Kernel::_readSet, &Kernel::_writeSet,
			0, &timeout) < 0) 
		{	
			if (!this->_exit)
				Logger::getInstance().log(ERROR, "select");					
			continue;
		}		
		if (this->_exit)
			break;
		this->callCatch();
		std::for_each(this->_servers.begin(), this->_servers.end(),
			this->callListen);
		std::for_each(this->_servers.begin(), this->_servers.end(),
			this->callReply);
		usleep(100);	
	}	
}

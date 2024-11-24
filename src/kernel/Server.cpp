#include "Server.hpp"
#include "Logger.hpp" 
#include "ConfigFileParser.hpp"

bool Server::_nl = false;

Server::Server(sockaddr_in & sockAddr, Config & conf)	
{
	static int i;		
	this->_conf = conf;		
	this->_conf.index = i++;
	this->_sockAddr = sockAddr;
	this->_clients.reserve(static_cast<size_t>(this->_conf.maxClient));
	this->_readBuffer.reserve(RECV_BUFF_SIZE);	
	this->_writeBuffer.reserve(SEND_BUFF_SIZE);
}

const sockaddr_in & Server::getSockAdress() const
{
	return (this->_sockAddr);
}

bool Server::setup()
{	
	this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (this->_fd < 0)	
		return Logger::getInstance().log(ERROR, "socket"), false;
	Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fd);
	FD_SET(this->_fd, &Kernel::_actualSet);
	if (bind(this->_fd, reinterpret_cast<const sockaddr *>
		(&this->_sockAddr), sizeof(this->_sockAddr)) < 0)
	{		
		Logger::getInstance().log(ERROR, "bind", *this);

		this->exitServer();		
		return false;		
	}	
	if (listen(this->_fd, this->_conf.maxClient) < 0)
	{
		Logger::getInstance().log(ERROR, "listen", *this);

		this->exitServer();
		return false;
	}
	Logger::getInstance().log(INFO, "listen", *this);
	return true;
}

void Server::catchClients(Kernel & kernel)
{
	if (FD_ISSET(this->_fd, &Kernel::_readSet))
	{	
		Server::_nl = !Server::_nl ? std::cout << std::endl, true : true;		
		Client client(&kernel._conf);			
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>
			(&client.address), &client.addressLen);			
		if (client.fd < 0)		
			return Logger::getInstance().log(ERROR, "accept");		
		if (kernel.countClients() >= this->_conf.maxClient)		
			return close(client.fd),
				Logger::getInstance().log(WARNING, "max clients", client);					
		Logger::getInstance().log(INFO, "\e[30;101mnew client\e[0m", client);
		struct timeval timeout = {SND_TIMEOUT, 0};	
		if (setsockopt(client.fd, SOL_SOCKET, SO_SNDTIMEO, &timeout,
			sizeof(timeout)) < 0)
	  		return close(client.fd),
				Logger::getInstance().log(ERROR, "send timeout", client);
		FD_SET(client.fd, &Kernel::_actualSet);
		Kernel::_maxFd = std::max(Kernel::_maxFd, client.fd);
		this->_clients.push_back(client);
	}	
}

void Server::exitClient(size_t i)
{
	Logger::getInstance().log(INFO, "\e[30;101mclient exited\e[0m",
		this->_clients[i]);

	FD_CLR(this->_clients[i].fd, &Kernel::_actualSet);
	close(this->_clients[i].fd);	
	this->_clients.erase(this->_clients.begin() + static_cast<int>(i));
}

void Server::exitClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		FD_CLR(this->_clients[i].fd, &Kernel::_actualSet);
		close(this->_clients[i].fd);	
	}
	this->_clients.clear();
}

void Server::exitServer()
{
	this->exitClients();
	FD_CLR(this->_fd, &Kernel::_actualSet);
	close(this->_fd);	
}

void Server::printVector(Client & client, const std::vector<char> & response,
	const std::string color, const int level)
{
	std::stringstream ss;
	ss << color << "Print data : " << YELLOW
		<< response.size() << " bytes" << color << std::endl << std::endl;
	ss << "-";		
	for (size_t i = 0; i < response.size(); i++)				
		ss << response[i];
	ss << "-\e[0m";
	Logger::getInstance().log(static_cast<logLevel>(level), ss.str(), client);
	if (Logger::getInstance()._logLevel[static_cast<logLevel>(level)])
		std::cout << std::endl;
}

void Server::shortCircuit(const e_errorCodes err, const size_t i)
{
	errorCode errCode;
	std::stringstream ss;	
	ss << "Short Circuit: " << RED << err << " " << errCode.getCode(err)
		<< RESET;
	Logger::getInstance().log(WARNING, ss.str(), this->_clients[i]);
	
	this->_clients[i].sendBuffer.clear();
	this->_clients[i].messageRecv.clear();
	this->_clients[i].responseBuilder = ResponseBuilder();
	this->_clients[i].responseBuilder.getHeader(this->_clients[i],
		this->_conf, err);
	this->_clients[i].ping = false;
	this->_clients[i].exitRequired = true;
}

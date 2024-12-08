#include "Server.hpp"
#include "Logger.hpp" 
#include "ConfigFileParser.hpp"

bool Server::_nl = false;

Server::Server(sockaddr_in & sockAddr, Config & conf): _conf(conf)
{
	static int i;
	this->_fd = -1;			
	this->_conf.index = i++;
	this->_sockAddr = sockAddr;
	this->_clients.reserve(static_cast<size_t>(this->_conf.maxClient));
	this->_readBuffer.reserve(conf.recv_buff_size);	
	this->_writeBuffer.reserve(conf.send_buff_size);
}

Server::Server(const Server & src): _conf(src._conf)
{
	this->_fd = -1;
	this->_clients.reserve(static_cast<size_t>(this->_conf.maxClient));
	this->_readBuffer.reserve(src._conf.recv_buff_size);	
	this->_writeBuffer.reserve(src._conf.send_buff_size);
	*this = src;	
}

Server & Server::operator=(const Server & rhs)
{
	this->_sockAddr = rhs._sockAddr;
	this->_writeBuffer = rhs._writeBuffer;
	this->_readBuffer = rhs._readBuffer;	
	this->_clients = rhs._clients;

	if (this->_fd >= 0)
	{
		FD_CLR(this->_fd, &Kernel::_actualSet);
		close(this->_fd);
	}
	if (rhs._fd >= 0)
	{
		if ((this->_fd = dup(rhs._fd)) >= 0)
			FD_SET(this->_fd, &Kernel::_actualSet);
		Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fd);
	}
	else
		this->_fd = rhs._fd;
	return *this;
}

Server::~Server()
{
	if (this->_fd < 0)
		return ;
	FD_CLR(this->_fd, &Kernel::_actualSet);
	close(this->_fd);	
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
		return Logger::getInstance().log(ERROR, "bind", *this), false;	
	if (listen(this->_fd, this->_conf.maxClient) < 0)
		return Logger::getInstance().log(ERROR, "listen", *this), false;
	Logger::getInstance().log(INFO, "listen", *this);
	return true;
}

void Server::catchClients(Kernel & kernel)
{
	if (FD_ISSET(this->_fd, &Kernel::_readSet))
	{	
		Server::_nl = !Server::_nl ? std::cout << std::endl, true : true;		
		Client client(&kernel._conf, this);			
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>
			(&client.address), &client.addressLen);			
		if (client.fd < 0)		
			return Logger::getInstance().log(ERROR, "accept");		
		if (kernel.countClients() >= this->_conf.maxClient)		
			return Logger::getInstance().log(WARNING, "max clients", client);					
		Logger::getInstance().log(INFO, "\e[30;101mnew client\e[0m", client);
		struct timeval timeout = {SND_TIMEOUT, 0};	
		if (setsockopt(client.fd, SOL_SOCKET, SO_SNDTIMEO, &timeout,
			sizeof(timeout)) < 0)
	  		return Logger::getInstance().log(ERROR, "send timeout", client);
		FD_SET(client.fd, &Kernel::_actualSet);
		Kernel::_maxFd = std::max(Kernel::_maxFd, client.fd);
		this->_clients.push_back(client);	
	}	
}

void Server::exitClient(size_t i)
{
	Logger::getInstance().log(INFO, "\e[30;101mclient exited\e[0m",
		this->_clients[i]);

	this->_clients.erase(this->_clients.begin() + static_cast<int>(i));
}

void Server::printVector(Client & client, const std::vector<char> & response,
	const std::string color, const int level)
{
	std::stringstream ss;
	ss << color << "Print data : " << HIGH_INTENSITY_YELLOW
		<< response.size() << " bytes" << color << std::endl << std::endl;
	ss << "-";

	for (size_t i = 0; i < response.size(); i++)
	{
		if (i > 1000)
		{
			ss << std::endl << HIGH_INTENSITY_YELLOW << "[...]" << color;
			break ;
		}
		if (response[i] == '\r')
		    ss << "\e[31m\\r\e[0m" << HIGH_INTENSITY_YELLOW;
		else if (response[i] == '\n')
		    ss << "\e[31m\\n\e[0m" << "\n" << HIGH_INTENSITY_YELLOW;
		else
		    ss << color << response[i];
	}
	ss << "-\e[0m";
	Logger::getInstance().log(static_cast<logLevel>(level), ss.str(), client);
	if (Logger::getInstance()._logLevel[static_cast<logLevel>(level)])	
		std::cerr << std::endl;	
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

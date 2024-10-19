
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <vector>
#include "Config.hpp"
#include <iostream>
#include "RequestParser.hpp"
#include <arpa/inet.h>

struct Client
{
	int					id;
	int					fd;
	std::vector<char>	request;
	sockaddr_in 		address;
	socklen_t 			len;

	Client()
	{
		id = 0;
		fd = 0;		
		memset(&address, 0, sizeof(address));
		len = sizeof(address);
	}
};

class Server
{
	Config 				_conf;
	sockaddr_in 		_sockAddr;
	std::vector<Client> _clients;
	std::vector<char>	_writeBuffer;
	std::vector<char>	_readBuffer;
	int					_fd;

	int	&				_maxFd;
	fd_set &			_actualSet;
	fd_set &			_readSet;
	fd_set &			_writeSet;
	
	RequestParser 		_parser;

	void exitClients();

	public:

		Server(sockaddr_in & sockaddr, int & maxFd, fd_set & actualSet, fd_set & readSet, fd_set & writeSet);
		void catchClients();
		void listenClients();
		void exitServer();
};
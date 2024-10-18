
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <vector>
#include <Config.hpp>
#include <iostream>

struct Client
{
	int					id;
	int					fd;
	std::vector<char>	request;
	sockaddr 			address;
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

	public:

		Server(sockaddr_in sockaddr, int & maxFd, fd_set & actualSet, fd_set & readSet, fd_set & writeSet);
		void catchClient();
		void listenClient();
};
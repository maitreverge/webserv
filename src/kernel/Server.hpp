
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <vector>
#include <Config.hpp>
#include <iostream>

struct Client
{
	int					id;
	std::vector<char>	request;
	sockaddr 			address;
};

class Server
{
	Config 				_conf;
	std::vector<Client> _clients;
	std::vector<char>	_write_buffer;
	std::vector<char>	_read_buffer;
	int					_fd;

	int	&				_maxFd;
	fd_set &			_actualSet;
	fd_set &			_readSet;
	fd_set &			_writeSet;

	public:
		Server(int & maxFd, fd_set & actualSet, fd_set & readSet, fd_set & writeSet);
		void catchClient();
};
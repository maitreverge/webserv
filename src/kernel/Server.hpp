
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

	public:
		Server(fd_set & actualSet, int & maxFd);
		void catchClient(int serverFd);
};
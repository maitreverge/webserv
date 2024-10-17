#include <iostream>
#include <Config.hpp>
#include <vector>
#include <sys/select.h>

// #include <netinet/in.h>
#include <sys/socket.h>

struct Client
{
	int					id;
	std::vector<char>	request;
	sockaddr 			address;
};

class Kernel
{
	Config 				_conf;
	std::vector<Client> _clients;
	fd_set				_actualSet;
	fd_set 				_readSet;
	fd_set 				_writeSet;
	std::vector<char>	_write_buffer;
	std::vector<char>	_read_buffer;
	int					_maxFd; 

	public:
		Kernel(void);
		~Kernel(void);
		Kernel(const Kernel & src);
		Kernel & operator=(const Kernel & rhs);	

		void setup();
		void catchClient(int);

};
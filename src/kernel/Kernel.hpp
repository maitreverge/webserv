#include <iostream>
#include <vector>
#include <sys/select.h>
#include <sys/socket.h>
#include <Server.hpp>

class Kernel
{
	Config 				_conf;
	std::vector<Server> _servers;
	fd_set				_actualSet;
	fd_set 				_readSet;
	fd_set 				_writeSet;
	int					_maxFd; 
	public:
		Kernel(void);
		~Kernel(void);
		Kernel(const Kernel & src);
		Kernel & operator=(const Kernel & rhs);	

		void setup();
		void catchClient(int);

};
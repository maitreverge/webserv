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
	Config 				conf;
	std::vector<Client> clients;
	fd_set				actualSet;
	fd_set 				readSet;
	fd_set 				writeSet;
	std::vector<char>	write_buffer;
	std::vector<char>	read_buffer;

	public:
		Kernel(void);
		~Kernel(void);
		Kernel(const Kernel & src);
		Kernel & operator=(const Kernel & rhs);	

		void setup();

};
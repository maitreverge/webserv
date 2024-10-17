#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

struct Config
{
	short int						maxClient;
	std::vector<struct sockaddr_in> sockAddress;

	Config()
	{
		maxClient = 1024;
		struct sockaddr_in server1;	
		std::memset(&server1, 0, sizeof(server1));
		server1.sin_family = AF_INET;
		server1.sin_addr.s_addr = htonl(INADDR_ANY);
		server1.sin_port = htons(1500);
		sockAddress.push_back(server1);
	}
};
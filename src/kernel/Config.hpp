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
		server1.sin_port = htons(1510);
		sockAddress.push_back(server1);

		struct sockaddr_in server2;	
		std::memset(&server2, 0, sizeof(server2));
		server2.sin_family = AF_INET;
		server2.sin_addr.s_addr = htonl(INADDR_ANY);
		server2.sin_port = htons(1511);
		sockAddress.push_back(server2);

		struct sockaddr_in server3;	
		std::memset(&server3, 0, sizeof(server3));
		server3.sin_family = AF_INET;
		server3.sin_addr.s_addr = htonl(INADDR_ANY);
		server3.sin_port = htons(1512);
		sockAddress.push_back(server3);
	}
};
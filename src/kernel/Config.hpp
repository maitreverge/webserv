#include <vector>
#include <netinet/in.h>
#include <sys.select.h>

struct Config
{
	std::vector<struct sockaddr_in> sockAddress;

	Config()
	{
		struct sockaddr_in server1;

		server1.sin_family = AF_INET
		sockAddress.push_back(server1);
	}

};
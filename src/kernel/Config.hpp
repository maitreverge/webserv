#pragma once

#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

struct Config
{
	short int						maxClient;
	std::vector<struct sockaddr_in> sockAddress;
	vector<string>					indexFiles; // default files names if the URI == "/"
	bool							listingDirectories; // activer ou non le listing des repertoires
	map<e_errorCodes, string>		errorPaths;

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

		struct sockaddr_in server4;	
		std::memset(&server4, 0, sizeof(server4));
		server4.sin_family = AF_INET;
		server4.sin_addr.s_addr = htonl(INADDR_ANY);
		server4.sin_port = htons(80);
		sockAddress.push_back(server4);

		// Default files to look for if the URI is "/"
		indexFiles.push_back("index.html");
		indexFiles.push_back("index.htm");
		indexFiles.push_back("default.html");

		listingDirectories = false;

		errorPaths.insert(std::make_pair(CODE_404_NOT_FOUND, "path/to/404.html"));
		errorPaths.insert(std::make_pair(CODE_403_FORBIDDEN, "path/to/403.html"));
		errorPaths.insert(std::make_pair(CODE_500_INTERNAL_SERVER_ERROR, "path/to/404.html"));
	}
};
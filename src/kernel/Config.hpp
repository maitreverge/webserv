#pragma once

#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "errorCode.hpp"

struct Config
{
	short int						maxClient;
	std::vector<struct sockaddr_in> sockAddress;
	vector<std::string>					indexFiles; // default files names if the URI == "/"
	bool							listingDirectories; // activer ou non le listing des repertoires
	map<e_errorCodes, string>		errorPaths;
	map<string, string> redirection; // Associate a Route = One Redirection
	map<string, map<string, string> > routeMapping; // Associate a Route = One mapping

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

		listingDirectories = true;

		// Error paths files
		errorPaths.insert(std::make_pair(CODE_400_BAD_REQUEST, "errorPages/400.html"));
		errorPaths.insert(std::make_pair(CODE_401_UNAUTHORIZED, "errorPages/401.html"));
		errorPaths.insert(std::make_pair(CODE_403_FORBIDDEN, "errorPages/403.html"));
		errorPaths.insert(std::make_pair(CODE_404_NOT_FOUND, "errorPages/404.html"));
		errorPaths.insert(std::make_pair(CODE_500_INTERNAL_SERVER_ERROR, "errorPages/500.html"));
		errorPaths.insert(std::make_pair(CODE_502_BAD_GATEWAY, "errorPages/502.html"));
		errorPaths.insert(std::make_pair(CODE_503_SERVICE_UNAVAILABLE, "errorPages/503.html"));
		errorPaths.insert(std::make_pair(CODE_504_GATEWAY_TIMEOUT, "errorPages/504.html"));

		// Redirections
		redirection.insert(std::make_pair(/* ONE ROUTE*/"/", /* ONE REDIRECTION*/"/testResponseBuilder/redirectDefaultPath/cats.html"));
		redirection.insert(std::make_pair("/nope", "/nopenopenope"));
		// ! Edge case => Tester Route A => Route B and Route B => Route A

		// Route mapping 1
		std::map<std::string, std::string> innerMap1;
		innerMap1.insert(std::make_pair("/coucou", "/testResponseBuilder/mapingPath"));
		routeMapping.insert(std::make_pair("/", innerMap1));

		std::map<std::string, std::string> innerMap2;
		innerMap2.insert(std::make_pair("/coucou", "/testResponseBuilder/mapingPath"));
		routeMapping.insert(std::make_pair("/testResponseBuilder/listingDirectory", innerMap2));
		
		
	}
};
#pragma once

#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "errorCode.hpp"

typedef map<std::string, map<std::string, std::vector<std::string> > > RoutesData;

struct server
{
	std::string	serverStructName;
	std::string	host;
	std::string	port;
	std::string	serverName;
	std::vector<std::string> allowedRoutes;
	RoutesData routesData;
};
 
/**========================================================================
 *                             CONFIG
 *========================================================================**/
struct Config
{
	bool							handleCookies;
	int								index;
	size_t							maxBodySize;
	ssize_t							maxHeaderSize;
	int								maxServerNbr;
	int								timeoutCgi;
	struct server					_serverStruct[8];
	short int						maxClient;
	std::vector<struct sockaddr_in> sockAddress;
	std::vector<std::string>		indexFiles;
	bool							listingDirectories;
	map<e_errorCodes, string>		errorPaths;	
	map<string, string>				redirection;
	std::string						errorPagesPath;
	RoutesData						routes;
	
	unsigned short					recv_buff_size;
	unsigned short					send_buff_size;

	Config();
	Config(char* path);
	void	initializeServers();
	void	initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress);
	void	intitializeVars();
};
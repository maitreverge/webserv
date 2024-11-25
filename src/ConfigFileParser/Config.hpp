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
 *?  modifs done:
 *?  string "errorPagesPath" added 
 *?  => concatenated to error pages file name
 *?  
 *========================================================================**/
struct Config
{
	int								index;
	size_t							maxBodySize;
	ssize_t							maxHeaderSize;
	int								maxServerNbr;
	int								timeoutCgi;
	struct server					_serverStruct[8];
	short int						maxClient;
	std::vector<struct sockaddr_in> sockAddress;
	std::vector<std::string>		indexFiles;			// default files names if the URI == "/"
	bool							listingDirectories; // activer ou non le listing des repertoires
	map<e_errorCodes, string>		errorPaths;	
	map<string, string>				redirection;
	std::string						errorPagesPath;
	RoutesData						routes;
	
	size_t							recv_buff_size;
	size_t							send_buff_size;

	Config();
	Config(char* path);
	void	initializeServers();
	void	initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress);
	void	intitializeVars(bool withConfigFile);
};
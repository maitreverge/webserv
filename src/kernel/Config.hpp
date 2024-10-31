#pragma once

#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "errorCode.hpp"

struct server
{
	std::string	host;
	std::string	port;
	std::string	serverName;
};

struct Config
{
	struct server					_serverStruct[8];
	short int						maxClient;
	std::vector<struct sockaddr_in> sockAddress;
	std::vector<std::string>		serverName; //inserer ici au meme indice que le server son servername
	std::vector<std::string>				indexFiles;			// default files names if the URI == "/"
	bool							listingDirectories; // activer ou non le listing des repertoires
	map<e_errorCodes, string>		errorPaths;	
	map<string, string>				redirection;
	std::string						errorPagesPath;

	Config();
	Config(char* path);
	void	initializeServers();
	void	initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress);
	void	intitializeVars(bool withConfigFile);
};
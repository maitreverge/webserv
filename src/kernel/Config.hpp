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
	// ! ======= GLOBAL SETTINGS ==========
	struct server					_serverStruct[8];
	short int						maxClient;
	std::vector<struct sockaddr_in> sockAddress;
	std::vector<std::string>		serverName; //inserer ici au meme indice que le server son servername
	map<e_errorCodes, string>		errorPaths;	

	// ! ======= ROUTE SETTINGS ==========
	map<string, map<string, vector<string> > > allowedMethods;
	map<string, map<string, vector<string> > > redirection;
	map<string, map<string, vector<string> > > routeMapping;
	map<string, map<string, vector<string> > > listingDirectory;
	map<string, map<string, vector<string> > > defaultPath;
	map<string, map<string, vector<string> > > cgiAllowed;
	map<string, map<string, vector<string> > > uploadAllowed;
	map<string, map<string, vector<string> > > uploadDirectory;

	Config();
	Config(char* path);
	void	initializeServer(server _serverStruct, std::vector<sockaddr_in>& sockAddress);
	void	initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress);

	void	buildKeys( string&, string&, vector<string>&, map<string, map< string, vector<string> > >& );
};
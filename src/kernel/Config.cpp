#include "Config.hpp"
#include "ConfigFileParser.hpp"
#include "master.hpp"

Config::Config(char *path) : maxBodySize(5000000), maxHeaderSize(8000), maxServerNbr(8)
{
	intitializeVars(1);
	ConfigFileParser parser;
	parser.parseConfigFile(*this, path);
	int i = 0;
	for (i = 0; i < maxServerNbr; i++)
		initializeServer((uint16_t)std::atoi(_serverStruct[i].port.c_str()), sockAddress);
}

Config::Config() : maxBodySize(5000000), maxHeaderSize(8000), maxServerNbr(8)
{
	intitializeVars(1);
	ConfigFileParser parser;
	parser.parseConfigFile(*this, (char *)("_configs/base_test/base_test_ok_1.ini"));
	int i = 0;
	for (i = 0; i < maxServerNbr; i++)
		initializeServer((uint16_t)std::atoi(_serverStruct[i].port.c_str()), sockAddress);
}

void	Config::intitializeVars(bool withConfigFile)
{
	if (!withConfigFile)
		initializeServers();
	
	maxClient = 1024;
	errorPagesPath = "errorPages/";
	// Default files to look for if the URI is "/"
	indexFiles.push_back("index.html");
	indexFiles.push_back("index.htm");
	indexFiles.push_back("default.html");

	listingDirectories = true;

	// Error paths files
	errorPaths.insert(std::make_pair(CODE_400_BAD_REQUEST, errorPagesPath + "400.html"));
	errorPaths.insert(std::make_pair(CODE_401_UNAUTHORIZED, errorPagesPath + "401.html"));
	errorPaths.insert(std::make_pair(CODE_403_FORBIDDEN, errorPagesPath + "403.html"));
	errorPaths.insert(std::make_pair(CODE_404_NOT_FOUND, errorPagesPath + "404.html"));
	errorPaths.insert(std::make_pair(CODE_500_INTERNAL_SERVER_ERROR, errorPagesPath + "500.html"));
	errorPaths.insert(std::make_pair(CODE_502_BAD_GATEWAY, errorPagesPath + "502.html"));
	errorPaths.insert(std::make_pair(CODE_503_SERVICE_UNAVAILABLE, errorPagesPath + "503.html"));
	errorPaths.insert(std::make_pair(CODE_504_GATEWAY_TIMEOUT, errorPagesPath + "504.html"));
}

void	Config::initializeServers()
{
	_serverStruct[0].host = "0.0.0.0";
	_serverStruct[0].port = "1510";
	_serverStruct[0].serverName = "server1";
	_serverStruct[1].host = "0.0.0.0";
	_serverStruct[1].port = "1511";
	_serverStruct[1].serverName = "server2";
	_serverStruct[2].host = "0.0.0.0";
	_serverStruct[2].port = "1512";
	_serverStruct[2].serverName = "server3";
	_serverStruct[3].host = "0.0.0.0";
	_serverStruct[3].port = "80";
	_serverStruct[3].serverName = "server4";

	std::vector<std::string> innerVector;
	innerVector.push_back("index.html");
	std::vector<std::string> innerVector2;
	innerVector.push_back("GET");
	std::vector<std::string> innerVector3;
	innerVector.push_back("/");

	std::map< std::string, std::vector<std::string> >innerMap;

	innerMap.insert(std::make_pair("index", innerVector));
	innerMap.insert(std::make_pair("allowedMethods", innerVector2));
	innerMap.insert(std::make_pair("uri", innerVector3));

	// std::map < std::string, std::map< std::string, std::vector<std::string> > >outterMap;

	_serverStruct[0].routesData.insert(std::make_pair("route1", innerMap));
	_serverStruct[1].routesData.insert(std::make_pair("route1", innerMap));
	_serverStruct[2].routesData.insert(std::make_pair("route1", innerMap));

	
	



	



	initializeServer((uint16_t)std::atoi(_serverStruct[0].port.c_str()), sockAddress);
	initializeServer((uint16_t)std::atoi(_serverStruct[1].port.c_str()), sockAddress);
	initializeServer((uint16_t)std::atoi(_serverStruct[2].port.c_str()), sockAddress);
	initializeServer(80, sockAddress);
}

/**========================================================================
 *                           INITIALIZESERVER
 * ? ugly dirty check added: do I need to default open port 80?
 * ? now I need to decide were to add the serverName in the data struct...
 *========================================================================**/
void	Config::initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress)
{
	//! logic to be inserted here

	if (port)
	{
		struct sockaddr_in server;	
		std::memset(&server, 0, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(port);
		sockAddress.push_back(server);
	}
}

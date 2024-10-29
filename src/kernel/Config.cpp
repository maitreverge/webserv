#include "Config.hpp"
#include "ConfigFileParser.hpp"
#include "master.hpp"

Config::Config()
{
	ConfigFileParser::printServerData(_serverStruct, 8);
	ConfigFileParser toto;
	toto.parseConfigFile(*this, (char *)"config.ini");
	printColor(RED, "STRUCT CONTENT:");
	for (int i = 0; i < 8; i++)
	{
		print(_serverStruct[i].host);
		print(_serverStruct[i].port);
		print(_serverStruct[i].serverName);
	}


	// _serverStruct[0].host = "0.0.0.0";
	// _serverStruct[0].port = "1510";
	// _serverStruct[0].serverName = "server1";
	// _serverStruct[1].host = "0.0.0.0";
	// _serverStruct[1].port = "1511";
	// _serverStruct[1].serverName = "server2";
	// _serverStruct[2].host = "0.0.0.0";
	// _serverStruct[2].port = "1512";
	// _serverStruct[2].serverName = "server3";

	maxClient = 1024;
	for (int i = 0; i < 8; i++)
	{
		initializeServer(_serverStruct[0], sockAddress);
		initializeServer(_serverStruct[1], sockAddress);
		initializeServer(_serverStruct[2], sockAddress);
	}
	initializeServer(80, sockAddress);
	
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
}

void	Config::initializeServer(server _serverStruct, std::vector<sockaddr_in>& sockAddress)
{
	struct sockaddr_in server;	
	std::memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons((uint16_t)std::atoi(_serverStruct.port.c_str()));
	sockAddress.push_back(server);
}

void	Config::initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress)
{
	struct sockaddr_in server;	
	std::memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	sockAddress.push_back(server);
}

// Config::Config(Config& other)
// {
// 	_

// }
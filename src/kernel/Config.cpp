#include "Config.hpp"
#include "ConfigFileParser.hpp"
#include "master.hpp"

Config::Config(char *path)
{
	std::cout << "test" << this->maxClient << std::endl;
	Config(); // a debugger
	std::cout << "test2" << this->maxClient << std::endl;
	ConfigFileParser toto;
	toto.parseConfigFile(*this, path);

	for (int i = 0; i < 8; i++)
	{
		initializeServer((uint16_t)std::atoi(_serverStruct[i].port.c_str()), sockAddress);
	}
	initializeServer(80, sockAddress);
	// toto.printConfig(*this);
}

Config::Config()
{
	// var serverName a passer a la structure Client
	// => dans le constructeur de Client, prendre le fichier de Conf en argument
	maxClient = 1024;
	_serverStruct[0].host = "0.0.0.0";
	_serverStruct[0].port = "1510";
	_serverStruct[0].serverName = "server1";
	_serverStruct[1].host = "0.0.0.0";
	_serverStruct[1].port = "1511";
	_serverStruct[1].serverName = "server2";
	_serverStruct[2].host = "0.0.0.0";
	_serverStruct[2].port = "1512";
	_serverStruct[2].serverName = "server3";

	initializeServer((uint16_t)std::atoi(_serverStruct[0].port.c_str()), sockAddress);
	initializeServer((uint16_t)std::atoi(_serverStruct[1].port.c_str()), sockAddress);
	initializeServer((uint16_t)std::atoi(_serverStruct[2].port.c_str()), sockAddress);
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
	ConfigFileParser toto;
	// toto.printConfig(*this);
}

void	Config::initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress)
{
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

#include "Config.hpp"
#include "ConfigFileParser.hpp"
#include "master.hpp"

Config::Config(char *path)
{
	// std::cout << "test" << this->maxClient << std::endl;
	intitializeVars(1);
	// std::cout << "test2" << this->maxClient << std::endl;
	ConfigFileParser toto;
	toto.parseConfigFile(*this, path);
	int i = 0;
	for (i = 0; i < 8; i++)
		initializeServer((uint16_t)std::atoi(_serverStruct[i].port.c_str()), sockAddress, i);
	// toto.printConfig(*this);
	// print("in Config func: ");
	// ConfigFileParser::printRoutesData(this->routes);
	
}

Config::Config()
{
	// var serverName a passer a la structure Client
	// => dans le constructeur de Client, prendre le fichier de Conf en argument
	intitializeVars(0);

	ConfigFileParser toto;
	// toto.printConfig(*this);
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
	initializeServer((uint16_t)std::atoi(_serverStruct[0].port.c_str()), sockAddress, 0);
	initializeServer((uint16_t)std::atoi(_serverStruct[1].port.c_str()), sockAddress, 1);
	initializeServer((uint16_t)std::atoi(_serverStruct[2].port.c_str()), sockAddress, 2);
	initializeServer(80, sockAddress, 3);
}

/**========================================================================
 *                           INITIALIZESERVER
 * ? ugly dirty check added: do I need to default open port 80?
 * ? now I need to decide were to add the serverName in the data struct...
 *========================================================================**/
void	Config::initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress, int i)
{
	(void)i;
	//! logic to be inserted here
	// print("toto");
	// if (!this->_serverStruct[i].serverStructName.empty())
	// 	printColor(RED, "SERVER NAME: " + this->_serverStruct[i].serverStructName);
	if (port)
	{
		struct sockaddr_in server;	
		std::memset(&server, 0, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(port);
		sockAddress.push_back(server);
	}
	_serverStruct[i].serverId = i;
	my_routes[i] = &_serverStruct[i].routesData;
	if (!_serverStruct[i].host.empty())
	{
		printColor(RED, "serverStruct");
		ConfigFileParser::printServerData(_serverStruct, 4);
		printColor(RED, "Config");
		ConfigFileParser::printRoutesData(*my_routes[i]);
	}
}

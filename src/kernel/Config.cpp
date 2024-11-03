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
	toto.printConfig(*this);
}

void	Config::buildKeys( string& route, string& subKey, vector<string>& innerVector, map<string, map< string, vector<string> > >&target ){

	map<string, vector<string> > innerMap;

	// Link the first sub_map
	innerMap.insert(std::make_pair( subKey, innerVector ));

	// Link the global Map
	target.insert( std::make_pair (route, innerMap));

	innerVector.clear();
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
	errorPaths.insert(std::make_pair(CODE_201_CREATED, "errorPages/201.html"));
	errorPaths.insert(std::make_pair(CODE_204_NO_CONTENT, "errorPages/204.html"));
	errorPaths.insert(std::make_pair(CODE_400_BAD_REQUEST, "errorPages/400.html"));
	errorPaths.insert(std::make_pair(CODE_401_UNAUTHORIZED, "errorPages/401.html"));
	errorPaths.insert(std::make_pair(CODE_403_FORBIDDEN, "errorPages/403.html"));
	errorPaths.insert(std::make_pair(CODE_404_NOT_FOUND, "errorPages/404.html"));
	errorPaths.insert(std::make_pair(CODE_413_PAYLOAD_TOO_LARGE, "errorPages/413.html"));
	errorPaths.insert(std::make_pair(CODE_422_UNPROCESSABLE_ENTITY, "errorPages/422.html"));
	errorPaths.insert(std::make_pair(CODE_431_REQUEST_HEADER_FIELDS_TOO_LARGE, "errorPages/431.html"));
	errorPaths.insert(std::make_pair(CODE_500_INTERNAL_SERVER_ERROR, "errorPages/500.html"));
	errorPaths.insert(std::make_pair(CODE_502_BAD_GATEWAY, "errorPages/502.html"));
	errorPaths.insert(std::make_pair(CODE_503_SERVICE_UNAVAILABLE, "errorPages/503.html"));
	errorPaths.insert(std::make_pair(CODE_504_GATEWAY_TIMEOUT, "errorPages/504.html"));

	// ! ================ ROUTE SETTINGS ================
	/*
		I tried to replicate the  map<  string,   map<  string,   vector<  string> > > Data; structure
		by initializing it by hand.

		Each sub config will be under a different name, but still remain for the same original structure
		I need explicit names from now, but my methods will only need a name change once we'll rely on the _data structure
	*/
	// Route names (main keys)
	
	string route0 = "/";
	string route1 = "/route1";
	string route2 = "/route2";

	// Sub Keys
	string subkey_allowedmethods = "allowedMethods";
	string subkey_routemapping = "routeMapping";
	
	// Inner Vector
	vector<string> innerVector;

	// Allowed Methods
	{
		innerVector.push_back("GET");
		buildKeys(route0, subkey_allowedmethods, innerVector, this->allowedMethods);
		innerVector.push_back("POST");
		innerVector.push_back("DELETE");
		buildKeys(route1, subkey_allowedmethods, innerVector, this->allowedMethods);
	}

	// ! ================ ROUTE SETTINGS ================

	#ifdef UNIT_TEST
	#else
	ConfigFileParser toto;
	toto.printConfig(*this);
	#endif

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

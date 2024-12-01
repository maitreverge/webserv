#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void ResponseBuilder::serverNameChecking( void )
{
	// Extracting ==> festival.localhost:3112
	string curHost = _client->headerRequest.getHeaders().Host;

	// Extracting ==> 3112
	string curPort = curHost.substr(curHost.find_last_of(":") + 1);

	// Extracting ==> festival
	string curServerName = curHost.substr(0, curHost.find_last_of("."));

	map< string, string > mapPort;

	printColor(BOLD_RED, curHost);

	for (int i = 0; i < _config->maxServerNbr; ++i)
	{
		string serverPort = _config->_serverStruct[i].port;
		string serverName = _config->_serverStruct[i].serverName;

		mapPort.insert(std::make_pair(serverPort, serverName));
    }

	// string matchPortName = mapPort.at(curPort)
	if (mapPort.at(curPort) == curServerName)
	{
		// matching with the struct
		if (_servernameType.find(curServerName) != _servernameType.end())
		{
			_serverName = _servernameType.at(curServerName);
			_isServerName = true;
		}
	}
	_serverNameChecked = true;




	if (not _serverNameChecked)
	{

	}

	/*
	======= PRINT CUR HOST =======
	festival.localhost:3112
		festival
		cyborg
		villa





		3012
		3112
		3212
	*/
}
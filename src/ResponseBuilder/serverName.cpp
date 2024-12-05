#include "ResponseBuilder.hpp"
#include "Logger.hpp"

string ResponseBuilder::parseServerName( string &str ){

	// Remove the double-quotes from  "  "festival"  "" within the config file.
	return ( str.substr(str.find_first_of("\"") + 1, str.find_last_of("\"") - 1) );
}

void ResponseBuilder::extractServerName()
{
	// Extracting ==> festival.localhost:3112
	string curHost = _client->headerRequest.getHeaders().Host;

	// Extracting ==> 3112
	string curPort = curHost.substr(curHost.find_last_of(":") + 1);

	// Extracting ==> festival
	string curServerName;
	if (curHost.find(".localhost:") != std::string::npos)
		curServerName = curHost.substr(0, curHost.find_last_of("."));
	else
		curServerName = curHost.substr(0, curHost.find_first_of(":"));

	map<string, string> mapPort;

	// Linking every port with every serverName
	for (int i = 0; i < _config->maxServerNbr; ++i)
	{
		string serverPort = _config->_serverStruct[i].port;
		string serverName = parseServerName(_config->_serverStruct[i].serverName);

		mapPort.insert(std::make_pair(serverPort, serverName));
	}

	if (mapPort.at(curPort) == curServerName)
	{
		// Looking if the current Servername in Host matches the required global serverName in coplianFormData.cpp
		if (_servernameType.find(curServerName) != _servernameType.end())
		{
			_serverName = _servernameType.at(curServerName);

			Logger::getInstance().log(INFO, "serverName detected");

			_isServerName = true;
		}
	}
	_serverNameChecked = true;
}

void ResponseBuilder::applyServerName( void )
{
	Logger::getInstance().log(DEBUG, "FUNCTION CALL :ResponseBuilder::applyServerName");

	// Erase the URI starting with '/'
	if (*_realURI.begin() == '/')
	{
		_realURI.erase(_realURI.begin());
	}

	// Append the _serverName with this one is not found in the URI
	if (_realURI.find(_serverName) == std::string::npos)
	{
		Logger::getInstance().log(DEBUG, "_realURI before serverName" + _realURI);

		_realURI.insert(0, _serverName);

		Logger::getInstance().log(DEBUG, "_realURI after serverName" + _realURI);
	}
}

void ResponseBuilder::serverNameChecking( void )
{
	if (not _serverNameChecked)
		extractServerName();
	if (_isServerName)
		applyServerName();
}
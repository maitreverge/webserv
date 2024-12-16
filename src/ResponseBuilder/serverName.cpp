/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverName.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:50 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:08:51 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseBuilder.hpp"
#include "Logger.hpp"

/**
 * @brief Parses the server name from a given string by removing the surrounding double quotes.
 *
 * This function takes a reference to a string, finds the first and last double quotes,
 * and returns the substring between them.
 *
 * @param str The input string containing the server name enclosed in double quotes.
 * @return A string representing the server name without the surrounding double quotes.
 */
string ResponseBuilder::parseServerName( string &str ){

	// Remove the double-quotes from  "  "festival"  "" within the config file.
	return ( str.substr(str.find_first_of("\"") + 1, str.find_last_of("\"") - 1) );
}

/**
 * @brief Extracts the server name from the client's request header and verifies it against the server configuration.
 * 
 * This function extracts the host and port from the client's request header, then maps the port to the server name
 * using the server configuration. It checks if the extracted server name matches the required global server name.
 * If a match is found, it sets the server name and updates the server name status.
 */
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

			Logger::getInstance().log(INFO, "ServerName Detected", L_VRB);

			_isServerName = true;
		}
	}
	_serverNameChecked = true;
}

/**
 * @brief Applies the server name to the URI if it is not already present.
 * 
 * This function modifies the _realURI member variable by performing the following steps:
 * 1. If the URI starts with a '/', it removes this character.
 * 2. If the server name (_serverName) is not found within the URI, it prepends the server name to the URI.
 * 
 * The function logs the process at the DEBUG level using the Logger instance.
 */
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

/**
 * @brief Checks and applies the server name if it has not been checked yet.
 * 
 * This function first checks if the server name has already been checked. If not, it extracts the server name.
 * If the server name is valid, it applies the server name.
 */
void ResponseBuilder::serverNameChecking( void )
{
	if (not _serverNameChecked)
		extractServerName();
	if (_isServerName)
		applyServerName();
}
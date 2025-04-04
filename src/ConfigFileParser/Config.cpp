/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:07:10 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:07:11 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ConfigFileParser.hpp"

/**========================================================================
 *                           CONSTRUCTORS
 *========================================================================**/
Config::Config(char *path) : handleCookies(false)
{
	intitializeVars();
	ConfigFileParser parser;
	parser.parseConfigFile(*this, path);
	int i = 0;
	for (i = 0; i < maxServerNbr; i++)
		initializeServer((uint16_t)std::atoi(_serverStruct[i].port.c_str()), sockAddress);
}

Config::Config()
{

	intitializeVars();
	ConfigFileParser parser;
	parser.parseConfigFile(*this, (char *)("_configs/master_config.ini")); // TODO : Relinker la vraie config par defaut
	int i = 0;
	for (i = 0; i < maxServerNbr; i++)
		initializeServer((uint16_t)std::atoi(_serverStruct[i].port.c_str()), sockAddress);
}

/**========================================================================
 *                           INTITIALIZEVARS
 * ? this function, if commented, breaks all tests
 * ? although it is in theory not usefull any more...
 *========================================================================**/
void	Config::intitializeVars()
{
	maxClient = 1024;
	maxHeaderSize = 8192;
	maxBodySize = 10000;
	maxServerNbr = 8;
	timeoutCgi = 20;
	send_buff_size = 4192;
	recv_buff_size = 4192;
	handleCookies = 0;
	errorPagesPath = "errorPages/";
	indexFiles.push_back("index.html");
	indexFiles.push_back("index.htm");
	indexFiles.push_back("default.html");
	// listingDirectories = true;

	errorPaths.insert(std::make_pair(CODE_242_CONNECTION, errorPagesPath + "242.html"));
	errorPaths.insert(std::make_pair(CODE_400_BAD_REQUEST, errorPagesPath + "400.html"));
	errorPaths.insert(std::make_pair(CODE_401_UNAUTHORIZED, errorPagesPath + "401.html"));
	errorPaths.insert(std::make_pair(CODE_403_FORBIDDEN, errorPagesPath + "403.html"));
	errorPaths.insert(std::make_pair(CODE_404_NOT_FOUND, errorPagesPath + "404.html"));
	errorPaths.insert(std::make_pair(CODE_500_INTERNAL_SERVER_ERROR, errorPagesPath + "500.html"));
	errorPaths.insert(std::make_pair(CODE_502_BAD_GATEWAY, errorPagesPath + "502.html"));
	errorPaths.insert(std::make_pair(CODE_503_SERVICE_UNAVAILABLE, errorPagesPath + "503.html"));
	errorPaths.insert(std::make_pair(CODE_504_GATEWAY_TIMEOUT, errorPagesPath + "504.html"));
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

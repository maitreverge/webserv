/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:07:30 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:07:31 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "errorCode.hpp"
#include "master.hpp"

typedef map<std::string, map<std::string, std::vector<std::string> > > RoutesData;

struct server
{
	std::string	serverStructName;
	std::string	host;
	std::string	port;
	std::string	serverName;
	std::vector<std::string> allowedRoutes;
	RoutesData routesData;
};
 
struct Config
{
	bool							handleCookies;
	int								index;
	size_t							maxBodySize;
	ssize_t							maxHeaderSize;
	int								maxServerNbr;
	int								timeoutCgi;
	struct server					_serverStruct[8];
	short int						maxClient;
	std::vector<struct sockaddr_in> sockAddress;
	std::vector<std::string>		indexFiles;			// default files names if the URI == "/"
	bool							listingDirectories; // activer ou non le listing des repertoires
	map<e_errorCodes, string>		errorPaths;	
	map<string, string>				redirection;
	std::string						errorPagesPath;
	RoutesData						routes;
	
	unsigned long					recv_buff_size;
	unsigned long					send_buff_size;

	Config();
	Config(char* path);
	void	initializeServer(uint16_t port, std::vector<sockaddr_in>& sockAddress);
	void	intitializeVars();
};
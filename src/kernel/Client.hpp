/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:57 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:09:58 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "errorCode.hpp"
#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"
#include "Server.hpp"
#include "Cookies.hpp"

struct Client
{
	Config *			conf;
	int					fd;
	sockaddr_in 		address;
	socklen_t 			addressLen;

	std::vector<char>	messageRecv;
	RequestParser		headerRequest;
	size_t				bodySize;
	ssize_t				chunkSize;
	bool				retryChunked;
	
	ResponseBuilder		responseBuilder;
	std::vector<char>	headerRespons;
	std::vector<char>	messageSend;
	std::vector<char>	sendBuffer;
	bool				sendFlag;
	
	short int			ping;
	bool 				pongHeader;
	bool				exitRequired;

	void 				clone(const Client & rhs);

	Client(Config *conf, Server* server);
	Client(const Client & src);
	Client & operator=(const Client & rhs);
	~Client();
	
	Cookies				cookies;
	Server*				_server;
	bool	isConnected() const;
};
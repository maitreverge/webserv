#pragma once

#include <vector>
#include "errorCode.hpp"
#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"

struct Client
{
	int					fd;
	std::vector<char>	messageRecv;
	std::vector<char>	messageSend;
	sockaddr_in 		address;
	socklen_t 			addressLen;
	size_t				bodySize;
	e_errorCodes	 	statusCodes;
	RequestParser		headerRequest;
	ResponseBuilder		responseBuilder;
	std::vector<char>	headerRespons;

	bool				ping;
	bool 				respHeader;
	
	Client();
	Client(const Client &);
	~Client();
};
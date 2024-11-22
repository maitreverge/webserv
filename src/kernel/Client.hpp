#pragma once

#include <vector>
#include "errorCode.hpp"
#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"

struct Client
{
	Config *			conf;
	int					fd;
	sockaddr_in 		address;
	socklen_t 			addressLen;

	std::vector<char>	messageRecv;
	RequestParser		headerRequest;
	size_t				bodySize;
	ssize_t				chunkedSize;
	bool				retryChunked;
	
	ResponseBuilder		responseBuilder;
	std::vector<char>	headerRespons;
	std::vector<char>	messageSend;
	std::vector<char>	sendBuffer;
	bool				sendFlag;
	
	short int			ping;
	bool 				pongHeader;
	bool				exitRequired;
	
	Client(Config *);	
};
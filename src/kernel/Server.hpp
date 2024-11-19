#pragma once

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <arpa/inet.h>
#include <algorithm>
#include "Config.hpp"
#include "RequestParser.hpp"
#include "errorCode.hpp"
#include "ResponseBuilder.hpp"

class Kernel;

class Server
{
	Config 				_conf;
	sockaddr_in			_sockAddr;
	std::vector<char>	_writeBuffer;
	std::vector<char>	_readBuffer;
	
	void clientMessage(size_t i, ssize_t ret);
	void displayClient(Client & client) const;
	void reSend(size_t i);
	void handleClientHeader(size_t i, ssize_t ret);
	void getRespHeader(size_t i);
	void bodyCheckin(size_t i);
	void handleClientBody(size_t i, ssize_t ret);
	bool isDelimiterFind(std::string delimiter, size_t i,
		std::vector<char>::iterator & it);
	void shortCircuit(e_errorCodes err, size_t i);
	bool isMaxHeaderSize(std::vector<char>::iterator it, size_t i);
	bool isContentLengthValid(size_t i);
	bool isBodyEnd(size_t i);
	void sendBodyPart(size_t i);
	void sendBodyEnd(size_t i);
	bool isBodyTooLarge(size_t i);
	bool isChunked(size_t i);
	void printResponse(const std::vector<char> & response);
	bool replyClient(size_t i, std::vector<char> & response);
	bool fillMessageSend(size_t i);
	bool endReply(size_t i);
	void exitClient(size_t index);
	void exitClients();

	public:

		int					_fd;
		std::vector<Client> _clients;

		Server(sockaddr_in & sockaddr, Config & conf);

		const sockaddr_in & getSockAdress() const;

		bool setup();
		void catchClients(Kernel & kernel);
		void listenClients();
		void replyClients();
		void exitServer();
};
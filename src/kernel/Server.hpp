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

#define RECV_BUFF_SIZE 4096
#define SEND_BUFF_SIZE 8

#define MAX_HDR_SIZE 8192
#define MAX_CNT_SIZE 30000000

class Server
{
	Config 				_conf;
	sockaddr_in 		_sockAddr;
	std::vector<Client> _clients;
	std::vector<char>	_writeBuffer;
	std::vector<char>	_readBuffer;
	int					_fd;

	int	&				_maxFd;
	fd_set &			_actualSet;
	fd_set &			_readSet;
	fd_set &			_writeSet;

	void clientMessage(size_t i, ssize_t ret);
	void displayClient(Client & client) const;
	void handleClientHeader(size_t i, ssize_t ret);
	void getRespHeader(size_t i);
	void handleClientBody(size_t i, ssize_t ret);
	bool isDelimiterFind(size_t i, std::vector<char>::iterator & it);
	void errorShortCircuit(e_errorCodes err, size_t i);
	bool isMaxHeaderSize(std::vector<char>::iterator it, size_t i);
	bool isContentLengthValid(size_t i);
	bool isBodyTerminated(size_t i);
	bool isBodyTooLarge(size_t i);
	void printResponse(const std::vector<char> & response);
	bool replyClient(size_t i, std::vector<char> & response,
		ssize_t repSize);
	bool endReply(size_t i);
	void exitClient(size_t index);
	void exitClients();

	public:

		Server(sockaddr_in & sockaddr, int & maxFd, fd_set & actualSet,
			fd_set & readSet, fd_set & writeSet);

		const sockaddr_in & getSockAdress() const;

		bool setup();
		void catchClients();
		void listenClients();
		void replyClients();
		void exitServer();
};
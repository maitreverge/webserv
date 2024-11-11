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

#define RECV_BUFF_SIZE 4000
#define SEND_BUFF_SIZE 4000

#define MAX_HDR_SIZE 8192
#define MAX_CNT_SIZE 30000000

class Kernel;
class Server
{
	sockaddr_in 		_sockAddr;
	std::vector<char>	_writeBuffer;
	std::vector<char>	_readBuffer;
	int					_fd;
	
	Config 				_conf;

	void clientMessage(size_t i, ssize_t ret);
	void displayClient(Client & client) const;
	void reSend(size_t i);
	void handleClientHeader(size_t i, ssize_t ret);
	void getRespHeader(size_t i);
	void bodyCheckin(size_t i);
	void handleClientBody(size_t i, ssize_t ret);
	bool isDelimiterFind(size_t i, std::vector<char>::iterator & it);
	void shortCircuit(e_errorCodes err, size_t i);
	bool isMaxHeaderSize(std::vector<char>::iterator it, size_t i);
	bool isContentLengthValid(size_t i);
	bool isBodyTerminated(size_t i);
	bool isBodyTooLarge(size_t i);
	void printResponse(const std::vector<char> & response);
	bool replyClient(size_t i, std::vector<char> & response);
	bool fillMessageSend(size_t i);
	bool endReply(size_t i);
	void exitClient(size_t index);
	void exitClients();

	public:

		std::vector<Client> _clients;

		Server(sockaddr_in & sockaddr, Config & conf);

		const sockaddr_in & getSockAdress() const;

		bool setup();
		void catchClients(Kernel & kernel);
		void listenClients();
		void replyClients();
		void exitServer();
};
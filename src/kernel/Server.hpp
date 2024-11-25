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
	static bool			_nl;
	
	bool recevData(const size_t i);
	void clientMessage(const size_t i, ssize_t ret);
	void retrySend(const size_t i);
	void headerCheckin(const size_t i, ssize_t ret);
	void bodyCheckin(const size_t i, const size_t addBodysize);
	bool isDelimiterFind(std::string delimiter, const size_t i,
		std::vector<char>::iterator & it);
	void shortCircuit(const e_errorCodes err, const size_t i);
	void isMaxHeaderSize(std::vector<char>::iterator it, const size_t i);
	void isContentLengthValid(const size_t i);
	bool isBodyEnd(const size_t i);
	void sendBodyPart(const size_t i);
	void sendBodyEnd(const size_t i);
	void isBodyTooLarge(const size_t i);

	bool isChunked(const size_t i);
	bool firstDelim(const size_t i);
	void calculateChunkSize(const size_t i, std::vector<char>::iterator & it);
	bool secondDelim(const size_t i, std::vector<char>::iterator &it);
	bool isChunkPartComplete(const size_t i, std::vector<char>::iterator & it);

	void sendBuffering(const size_t i, std::vector<char> & response);
	bool replyClient(const size_t i, std::vector<char> & response);
	void fillMessageSend(const size_t i);
	bool endReply(const size_t i);
	void exitClient(const size_t index);
	void exitClients();

	public:

		int					_fd;
		std::vector<Client> _clients;

		Server(sockaddr_in & sockaddr, Config & conf);

		static void printVector(Client & client,
			const std::vector<char> & response,
			const std::string color = GREEN, const int level = 1);
		const sockaddr_in & getSockAdress() const;

		bool setup();
		void catchClients(Kernel & kernel);
		void listenClients();
		void replyClients();
		void exitServer();

		class ShortCircuitException
		{
			e_errorCodes _code;

			public:
				ShortCircuitException(e_errorCodes code):_code(code){}
				e_errorCodes getCode() const { return _code;}			
		};	
};
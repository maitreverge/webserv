#pragma once

#include <iostream>
#include <vector>
#include <sys/select.h>
#include <sys/socket.h>
#include <algorithm>
#include "Server.hpp"
#include "Logger.hpp"
#include "ConfigFileParser.hpp"

#define RECV_BUFF_SIZE 30000
#define SEND_BUFF_SIZE 30000
#define SND_TIMEOUT 1
#define SLCT_TIMEOUT 1

class Kernel
{
	std::vector<Server> _servers;

	void		callCatch();
	static void callListen(Server & server);
	static void callReply(Server & server);

	public:

		Config			_conf;
		static bool		_exit;
		static int		_maxFd; 
		static fd_set	_actualSet;
		static fd_set 	_readSet;
		static fd_set 	_writeSet;

		Kernel(void);
		Kernel(char* path);
		~Kernel();

		static Kernel & getInstance(char * = NULL);
	
		void		setup();
		void 		launch();
		short int	countClients();
};
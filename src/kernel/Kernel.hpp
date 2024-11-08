#pragma once

#include <iostream>
#include <vector>
#include <sys/select.h>
#include <sys/socket.h>
#include <algorithm>
#include "Server.hpp"
#include "Logger.hpp"
#include "ConfigFileParser.hpp"

#define MAX_SERVERS 42
#define MAX_CLIENTS 1000

class Server;

class Kernel
{
	Config 				_conf;
	std::vector<Server> _servers;


	static void callCatch(Server & server);
	static void callListen(Server & server);
	static void callReply(Server & server);
	static void callExit(Server & server);

	public:

	static bool		_exit;
	static int		_maxFd; 
	static fd_set	_actualSet;
	static fd_set 	_readSet;
	static fd_set 	_writeSet;
		
		Kernel(void);
		Kernel(char* path);

		void setup();
		void launch();
};
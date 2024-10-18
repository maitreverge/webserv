#include <iostream>
#include <vector>
#include <sys/select.h>
#include <sys/socket.h>
#include <Server.hpp>
#include <algorithm>
#include <csignal>
#include <termios.h>

class Kernel
{
	Config 				_conf;
	std::vector<Server> _servers;

	fd_set				_actualSet;
	fd_set 				_readSet;
	fd_set 				_writeSet;
	int					_maxFd; 

	static void callCatch(Server & server);
	static void callListen(Server & server);
	static void	signalHandle(int);
	void 		disableSignalEcho();

	public:

		static bool			_exit;
		Kernel(void);

		void setup();
		void launch();
};
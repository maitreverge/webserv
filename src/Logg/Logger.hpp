#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Server.hpp"
#include "RequestParser.hpp"
#include "timeStamp.hpp"
#include "Kernel.hpp"
#include "Client.hpp"
#include <ctime>
#include <arpa/inet.h>

class Kernel;
class Server;

typedef enum
{
	INFO = 0,
	DEBUG,
	WARNING,
	ERROR,
	LOG_LEVEL_COUNT
}	logLevel;

/**========================================================================
 *                             LOGGER
 *  LOG CONFIG: values to be changed in .cpp file:
 *?  Logger::Logger() : logToStdOut(1) 	=> 	0 to silence console log
 *?{
 *?	_logLevel[INFO] = 		1;			=>	0 to filter
 *?	_logLevel[DEBUG] = 		1;			=>	0 to filter
 *?	_logLevel[WARNING] = 	1;			=>	0 to filter
 *?	_logLevel[ERROR] = 		1;			=>	0 to filter
 *?	... 
 *========================================================================**/
class Logger
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif

		std::ofstream	_accessFile;
		std::ofstream	_errorFile;
		bool			logToStdOut;
		
		void		logOut(logLevel logLevel, const std::string& message);
		std::string	formatLogLevel(logLevel loglevel) const;
		std::string removeAnsiCodes(const std::string& message);
		int			portToInt(const struct sockaddr_in& addr);
		std::string	intToString(int value);
	public:
		static std::string ipToString(const struct sockaddr_in& addr);

		int	_logLevel[LOG_LEVEL_COUNT];

		Logger(int verbose = 1);
		~Logger();
		void log(logLevel logLevel, const std::string& message);
		void log(logLevel logLevel, const std::string& message, const RequestParser& context);
		void log(logLevel logLevel, const std::string& message, const Client& context);
		void log(logLevel logLevel, const std::string& message, const Client& client, bool yesNo);
		void log(logLevel logLevel, const std::string& message, const Server&server, bool yesNo);
		void log(logLevel logLevel, const std::string& message, const Client& client, const Server&obj);
		void log(logLevel logLevel, const std::string& message, const Server&server);
		static Logger& getInstance(int verbose = 1);
};


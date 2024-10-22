#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Server.hpp"
#include "RequestParser.hpp"
#include "Kernel.hpp"

class Kernel;
class Server;

typedef enum
{
	INFO,
	DEBUG,
	WARNING,
	ERROR
}	logLevel;

class Logger
{
private:
	std::ofstream	_accessFile;
	std::ofstream	_errorFile;
	std::string		getTimestamp();
	bool			logToStdOut;
	int				_logLevel;

public:
	Logger();
	~Logger();
	void log(logLevel logLevel, const std::string& message);
	void log(logLevel logLevel, const std::string& message, const Kernel& context);
	void log(logLevel logLevel, const std::string& message, const RequestParser& context);
	void log(logLevel logLevel, const std::string& message, const Server& context);
	static Logger& getInstance();

};

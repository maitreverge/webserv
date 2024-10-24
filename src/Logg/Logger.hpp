#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Server.hpp"
#include "RequestParser.hpp"
#include "Kernel.hpp"
#include "Error.hpp"

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
	bool			logToStdOut;
	int				_logLevel;

public:
	Logger();
	~Logger();
	void log(logLevel logLevel, const std::string& message);
	void log(logLevel logLevel, const std::string& message, const Kernel& context);
	void log(logLevel logLevel, const std::string& message, const RequestParser& context);
	void log(logLevel logLevel, const std::string& message, const Client& context);
	void log(logLevel logLevel, const std::string& message, const class buildResponse& obj);
	void log(logLevel logLevel, const std::string& message, const Client& client, const Server&obj);
	void log(logLevel logLevel, std::string& message, struct Client& client, const Error& error);
	void log(logLevel logLevel, const std::string& message, const Server&server);
	std::string	formatLogLevel(logLevel loglevel) const;
	static Logger& getInstance();

	std::string ipToString(const struct sockaddr_in& addr);
	int portToInt(const struct sockaddr_in& addr);
	std::string intToString(int value);
};


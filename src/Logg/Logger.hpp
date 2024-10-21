#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "RequestParser.hpp"
#include "Kernel.hpp"
#include "Server.hpp"

class Logger
{
private:
	std::ofstream	_accessFile;
	std::ofstream	_errorFile;
	std::string		getTimestamp();
	bool			isConsoleOutput;
	int				logLevel;

public:
	Logger();
	~Logger();

	void log(const std::string& message, const RequestParser& context);
	void log(const std::string& message, const Kernel& context);
	void log(const std::string& message, const Server& context);
	static Logger& getInstance();

};

#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Logger
{
private:
	std::ofstream	_accessFile;
	std::ofstream	_errorFile;
	std::string		getTimestamp();

public:
	Logger(const std::string& accessFile, const std::string& errorFile);
	~Logger();

	void log(const std::string& message);
	static Logger& getInstance(const std::string& accessFile, const std::string& errorFile);
};
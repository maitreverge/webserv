#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace Logg
{
	class Logger
	{
	private:
		std::ofstream _logFile;
		std::string getTimestamp();

	public:
		Logger(const std::string& fileName);
		~Logger();

		void log(const std::string& message);
		
		static Logger& getInstance(const std::string& fileName = "webserv.log");
	};
}

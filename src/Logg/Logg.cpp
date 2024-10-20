#include "Logg.hpp"
#include <ctime>

namespace Logg
{
	std::string Logger::getTimestamp()
	{
		std::time_t now = std::time(0);
		char buffer[80];
		std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
		return std::string(buffer);
	}
	Logger::Logger(const std::string& fileName)
	{
		_logFile.open(fileName.c_str(), std::ios::app);
		if (!_logFile.is_open()) {
			std::cerr << "Erreur : Impossible d'ouvrir le fichier de log." << std::endl;
		}
	}
	Logger::~Logger()
	{
		if (_logFile.is_open())
			_logFile.close();
	}
	void Logger::log(const std::string& message)
	{
		if (_logFile.is_open())
			_logFile << getTimestamp() << " - " << message << std::endl;
	}
	Logger& Logger::getInstance(const std::string& fileName)
	{
		static Logger instance(fileName); // Instance unique
		return instance;
	}
}

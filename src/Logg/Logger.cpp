#include "Logger.hpp"
#include <ctime>

/**========================================================================
 *                           CONSTRUCTOR && DESTRUCTOR
 *========================================================================**/
Logger::Logger() : logToStdOut(1), _logLevel(0)
{
	_accessFile.open("access.log", std::ios::app);
	_errorFile.open("error.log", std::ios::app);
	if (!_accessFile.is_open() || !_errorFile.is_open())
		Logger::getInstance().log(ERROR, "Erreur : Impossible d'ouvrir le fichier de log.");
	(void)_logLevel;
}

Logger::~Logger()
{
	if (_accessFile.is_open())
		_accessFile.close();
}

/**========================================================================
 *                           UTILS
 *========================================================================**/
std::string Logger::getTimestamp()
{
    return timeStamp::getTime();
}

/**========================================================================
 *                           ACTION
 *========================================================================**/
void Logger::log(logLevel logLevel, const std::string& message)
{
	std::string logEntry = getTimestamp() + " - " + message + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	_accessFile << logEntry;
	_accessFile.flush();
	(void)logLevel;
}

void Logger::log(logLevel logLevel, const std::string& message, const RequestParser& obj)
{
	std::string logEntry = getTimestamp() + " - " + message + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	_accessFile << logEntry;
	_accessFile.flush();
	(void)logLevel;
	(void)obj;
}

void Logger::log(logLevel logLevel, const std::string& message, const Kernel& obj)
{
	std::string logEntry = getTimestamp() + " - " + message + "\n";
	_accessFile << logEntry;
	if (logToStdOut)
		std::cout << logEntry;
	(void)obj;
	_accessFile.flush();
	(void)logLevel;
}

void Logger::log(logLevel logLevel, const std::string& message, const Server& obj)
{
	std::string logEntry = getTimestamp() + " - " + message + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
	_accessFile.flush();
	(void)obj;
	(void)logLevel;
}
/**========================================================================
 *                       SINGLETON ACCESS
 *========================================================================**/
Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}


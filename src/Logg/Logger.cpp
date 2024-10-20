#include "Logger.hpp"
#include <ctime>

/**========================================================================
 *                           CONSTRUCTOR && DESTRUCTOR
 *========================================================================**/
Logger::Logger(const std::string& accessFile, const std::string& errorFile)
{
	_accessFile.open(accessFile.c_str(), std::ios::app);
	_errorFile.open(errorFile.c_str(), std::ios::app);
	if (!_accessFile.is_open() || !_errorFile.is_open())
		std::cerr << "Erreur : Impossible d'ouvrir le fichier de log." << std::endl;
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
	std::time_t now = std::time(0);
	char buffer[80];
	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	return std::string(buffer);
}

/**========================================================================
 *                           ACTION
 *========================================================================**/
void Logger::log(const std::string& message, const RequestParser& obj)
{
	std::string logEntry = getTimestamp() + " - " + obj.getMethod() + message + "\n";
	if (isConsoleOutput)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
}

void Logger::log(const std::string& message, const Kernel& obj)
{
	std::string logEntry = getTimestamp() + " - " + message + "\n";
	if (isConsoleOutput)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
	(void)obj;
}

void Logger::log(const std::string& message, const Server& obj)
{
	std::string logEntry = getTimestamp() + " - " + message + "\n";
	if (isConsoleOutput)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
	(void)obj;
}
/**========================================================================
 *                       SINGLETON ACCESS
 *========================================================================**/
Logger& Logger::getInstance(const std::string& accessFile, const std::string& errorFile)
{
	static Logger instance(accessFile, errorFile);
	return instance;
}


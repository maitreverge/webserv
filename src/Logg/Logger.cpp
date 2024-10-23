#include "Logger.hpp"
#include <ctime>
#include <arpa/inet.h>  // pour inet_ntoa

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
 *                           ACTION
 *========================================================================**/
void Logger::log(logLevel logLevel, const std::string& message)
{
	
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message 
							+ RESET + "\n";	if (logToStdOut)
	std::cout << logEntry;
	_accessFile << logEntry;
	_accessFile.flush();
}

std::string Logger::formatLogLevel(logLevel level) const
{
	switch (level)
	{
		case INFO:
			return std::string(GREEN) + "[INFO] " + RESET;
		case WARNING:
			return std::string(YELLOW) + "[WARNING] " + RESET;
		case ERROR:
			return std::string(RED) + "[ERROR] " + RESET;
		default:
			return std::string(WHITE) + "[UNKNOWN] " + RESET;
	}
}

std::string Logger::ipToString(const struct sockaddr_in& addr)
{
    return std::string(inet_ntoa(addr.sin_addr));
}

int Logger::portToInt(const struct sockaddr_in& addr)
{
    return ntohs(addr.sin_port); // Convertit le port en host byte order
}

std::string Logger::intToString(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void Logger::log(logLevel logLevel, const std::string& message, const Kernel& obj)
{
	std::string logEntry = timeStamp::getTime() + " - " + message + "\n";
	_accessFile << logEntry;
	if (logToStdOut)
		std::cout << logEntry;
	(void)obj;
	_accessFile.flush();
	(void)logLevel;
}

void Logger::log(logLevel logLevel, const std::string& message, const RequestParser& obj)
{
	//[timestamp][loglevel][message][method][ip][port][fd]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							+ MAGENTA + ipToString(obj.getClient()->address) + " "
							+ YELLOW + intToString(portToInt(obj.getClient()->address)) + " "
							+ GREEN + intToString(obj.getClient()->fd)
							+ RESET + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	_accessFile << logEntry;
	_accessFile.flush();
	(void)logLevel;
	(void)obj;
}

void Logger::log(logLevel logLevel, const std::string& message, const Server& obj)
{
	//[timestamp][loglevel][message][ip][port][fd]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							// + MAGENTA + ipToString(obj.getClient()->address) + " "
							// + YELLOW + intToString(portToInt(obj.getClient()->address)) + " "
							// + GREEN + intToString(obj.getClient()->fd)
							+ RESET + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
	_accessFile.flush();
	(void)obj;
	(void)logLevel;
}

void Logger::log(	logLevel logLevel, const std::string& message,
					const class buildResponse& obj)
{
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							// + MAGENTA + ipToString(obj.getClient()->address) + " "
							// + YELLOW + intToString(portToInt(obj.getClient()->address)) + " "
							// + GREEN + intToString(obj.getClient()->fd)
							+ RESET + "\n";
	_accessFile << logEntry;
	if (logToStdOut)
		std::cout << logEntry;
	(void)obj;
	_accessFile.flush();
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


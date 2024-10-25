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

/**========================================================================
 *                           LOG KERNEL
 * ? to be filled as soon as I know which vars to display
 *========================================================================**/
void Logger::log(logLevel logLevel, const std::string& message, const Kernel& obj)
{
	//[timestamp][loglevel][message][ip][port][fd]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							// + MAGENTA + ipToString(client.address) + " "
							// + YELLOW + intToString(portToInt(client.address)) + " "
							// + GREEN + intToString(client.fd) + " "
							// + BOLD_HIGH_INTENSITY_WHITE + "Server: "
							// + MAGENTA + ipToString(server._sockAddr) + " "
							// + YELLOW + intToString(portToInt(server._sockAddr)) + " "
							+ RESET + "\n"
							;
	if (logToStdOut)
		std::cout << logEntry;
	(void)obj;
	_accessFile.flush();
}

void Logger::log(logLevel logLevel, const std::string& message, const RequestParser& obj)
{
	std::string logEntry;
	//[timestamp][loglevel][message][method][ip][port][fd]
	if (obj.getClient())
	{
		logEntry = 	BLUE + timeStamp::getTime() + ": " 
								+ formatLogLevel(logLevel) 
								+ BOLD_HIGH_INTENSITY_WHITE + message + " "
								+ MAGENTA + "Client: " + ipToString(obj.getClient()->address) + " "
								+ YELLOW + intToString(portToInt(obj.getClient()->address)) + " "
								+ GREEN + intToString(obj.getClient()->fd)
								+ RESET + "\n";
	}
	else
	{
		logEntry = 	BLUE + timeStamp::getTime() + ": " 
					+ formatLogLevel(logLevel) 
					+ BOLD_HIGH_INTENSITY_WHITE + message + " "
					+ RESET + "\n";
	}
	if (logToStdOut)
		std::cout << logEntry;
	_accessFile << logEntry;
	_accessFile.flush();
}

void Logger::log(logLevel logLevel, const std::string& message, const Client& client)
{
	//[timestamp][loglevel][message][ip][port][fd]
	std::string logEntry;
	logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd)
							+ RESET + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
	_accessFile.flush();
}

void Logger::log(logLevel logLevel, const std::string& message, const Client& client, const Server&server)
{
	//[timestamp][loglevel][message][ip][port][fd]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) + " "
							+ BOLD_HIGH_INTENSITY_WHITE + message
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd) + " "
							+ BOLD_HIGH_INTENSITY_WHITE
							+ MAGENTA + "Server: " + ipToString(server._sockAddr) + " "
							+ YELLOW + intToString(portToInt(server._sockAddr)) + " "
							+ RESET + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
	_accessFile.flush();
}

void Logger::log(logLevel logLevel, const std::string& message, const Server&server)
{
	//[timestamp][loglevel][message][ip][port]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							+ BOLD_HIGH_INTENSITY_WHITE
							+ MAGENTA + "Server: " + ipToString(server._sockAddr) + " "
							+ YELLOW + intToString(portToInt(server._sockAddr)) + " "
							+ RESET + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
	_accessFile.flush();
}

void Logger::log(logLevel logLevel, std::string& message, struct Client& client, const Error& error)
{
	// [timestamp][loglevel][message][ip][port]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							+ BOLD_HIGH_INTENSITY_WHITE
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd) + " "
							+ RESET + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	if (_accessFile.is_open())
		_accessFile << logEntry;
	_accessFile.flush();
	(void)error;
}

/**========================================================================
 *                           LOG BUILDRESPONSE
 * ? changes to be made as soon as Client struct is accessible
 *========================================================================**/
void Logger::log(	logLevel logLevel, const std::string& message,
					const class buildResponse& obj)
{
	//[timestamp][loglevel][message][status code][ip][port][fd]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							+ RED + obj.getStatusLine()
							// + MAGENTA + "Client: " + ipToString(obj.getClient()->address) + " "
							// + YELLOW + intToString(portToInt(obj.getClient()->address)) + " "
							// + GREEN + intToString(obj.getClient()->fd)
							+ RESET + "\n";
	_accessFile << logEntry;
	if (logToStdOut)
		std::cout << logEntry;
	(void)obj;
	_accessFile.flush();
}

/**========================================================================
 *                       SINGLETON ACCESS
 *========================================================================**/
Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}


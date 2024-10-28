#include "Logger.hpp"
#include <ctime>
#include <arpa/inet.h>  // pour inet_ntoa

/**========================================================================
 *                           CONSTRUCTOR && DESTRUCTOR
 *========================================================================**/
Logger::Logger() : logToStdOut(1)
{
	_logLevel[INFO] = 		1;
	_logLevel[DEBUG] = 		1;
	_logLevel[WARNING] = 	1;
	_logLevel[ERROR] = 		1;
	_accessFile.open("access.log", std::ios::app);
	_errorFile.open("error.log", std::ios::app);
	if (!_accessFile.is_open() || !_errorFile.is_open())
		Logger::getInstance().log(ERROR, "Erreur : Impossible d'ouvrir le fichier de log.");
}	

Logger::~Logger()
{
	if (_accessFile.is_open())
		_accessFile.close();
	if (_errorFile.is_open())
		_errorFile.close();
}

/**========================================================================
 *                           ACTION
 *========================================================================**/
void Logger::logOut(logLevel logLevel, const std::string& message)
{
	std::string logEntry;

	if (logToStdOut)
		std::cout << message;
	logEntry = removeAnsiCodes(message);
	if (logLevel == INFO || logLevel == DEBUG)
	{
		_accessFile << logEntry;
		_accessFile.flush();
	}
	else if (logLevel == WARNING || logLevel == ERROR)
	{
		_errorFile << logEntry;
		_errorFile.flush();
	}
}

void Logger::log(logLevel logLevel, const std::string& message)
{
	std::string logEntry;

	if (_logLevel[logLevel] == 0)
		return ;
	logEntry = 	BLUE + timeStamp::getTime() + ": " 
				+ formatLogLevel(logLevel) 
				+ GREEN + message 
				+ RESET + "\n";
	if (logToStdOut)
		std::cout << logEntry;
	logEntry = removeAnsiCodes(logEntry);
	if (logLevel == INFO || logLevel == DEBUG)
	{
		_accessFile << logEntry;
		_accessFile.flush();
	}
	else if (logLevel == WARNING || logLevel == ERROR)
	{
		_errorFile << logEntry;
		_errorFile.flush();
	}
}

std::string Logger::formatLogLevel(logLevel level) const
{
	switch (level)
	{
		case INFO:
			return std::string(GREEN) + "[INFO] " + RESET;
		case DEBUG:
			return std::string(BLUE) + "[DEBUG] " + RESET;
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

std::string Logger::removeAnsiCodes(const std::string& message)
{
	std::string cleanMessage;
	bool inAnsi = false;

	for (size_t i = 0; i < message.length(); ++i) {
		if (message[i] == '\033') {
			inAnsi = true;
		} else if (inAnsi && (message[i] == 'm' || message[i] == 'K')) {
			inAnsi = false;
		} else if (!inAnsi) {
			cleanMessage += message[i];
		}
	}
	return cleanMessage;
}

/**========================================================================
 *                           LOG KERNEL
 * ? to be filled as soon as I know which vars to display
 *========================================================================**/
void Logger::log(logLevel logLevel, const std::string& message, const Kernel& obj)
{
	if (_logLevel[logLevel] == 0)
		return ;
	//[timestamp][loglevel][message][ip][port][fd]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ GREEN + message + " "
							// + MAGENTA + ipToString(client.address) + " "
							// + YELLOW + intToString(portToInt(client.address)) + " "
							// + GREEN + intToString(client.fd) + " "
							// + GREEN + "Server: "
							// + MAGENTA + ipToString(server._sockAddr) + " "
							// + YELLOW + intToString(portToInt(server._sockAddr)) + " "
							+ RESET + "\n"
							;
		log(logLevel, logEntry);
	(void)obj;
}

void Logger::log(logLevel logLevel, const std::string& message, const RequestParser& obj)
{
	if (_logLevel[logLevel] == 0)
		return ;
	std::string logEntry;
	//[timestamp][loglevel][message][method][ip][port][fd]
	if (obj.getClient())
	{
		logEntry = 	BLUE + timeStamp::getTime() + ": " 
								+ formatLogLevel(logLevel) 
								+ GREEN + message + " "
								+ MAGENTA + "Client: " + ipToString(obj.getClient()->address) + " "
								+ YELLOW + intToString(portToInt(obj.getClient()->address)) + " "
								+ GREEN + intToString(obj.getClient()->fd)
								+ RESET + "\n";
	}
	else
	{
		logEntry = 	BLUE + timeStamp::getTime() + ": " 
					+ formatLogLevel(logLevel) 
					+ GREEN + message + " "
					+ RESET + "\n";
	}
	logOut(logLevel, logEntry);
}

void Logger::log(logLevel logLevel, const std::string& message, const Client& client)
{
	if (_logLevel[logLevel] == 0)
		return ;
	//[timestamp][loglevel][message][ip][port][fd]
	std::string logEntry;
	logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ GREEN + message + " "
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd)
							+ RESET + "\n";
	logOut(logLevel, logEntry);
}

void Logger::log(logLevel logLevel, const std::string& message, const Client& client, const Server&server)
{
	if (_logLevel[logLevel] == 0)
		return ;
	//[timestamp][loglevel][message][ip][port][fd]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) + " "
							+ GREEN + message
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd) + " "
							+ GREEN + "Server: "
							+ MAGENTA + ipToString(server.getSockAdress()) + " "
							+ YELLOW + intToString(portToInt(server.getSockAdress())) + " "
							+ RESET + "\n";
	logOut(logLevel, logEntry);
}
/**========================================================================
 *!                           DEBUG METHOD
 *========================================================================**/
void Logger::log(logLevel logLevel, const std::string& message, const Client& client, bool yesNo)
{
	if (_logLevel[logLevel] == 0)
		return ;
	//[timestamp][loglevel][message][ip][port][fd]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) + " "
							+ BOLD_HIGH_INTENSITY_WHITE + message
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd) + " "
							+ BOLD_HIGH_INTENSITY_WHITE + "Server: "
							+ RESET + "\n";
	

	// Convertir le fd en string pour nommer le fichier
    std::string filename = intToString(client.fd) + ".log";  
    std::ofstream logFile(filename.c_str());  
    
    if (logFile.is_open() && yesNo)
    {
        logFile << removeAnsiCodes(logEntry);
        logFile.close();
    } 
    else 
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier de log." << std::endl;
    }
}


void Logger::log(logLevel logLevel, const std::string& message, const Server&server)
{
	if (_logLevel[logLevel] == 0)
		return ;
	//[timestamp][loglevel][message][ip][port]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ GREEN + message + " "
							+ GREEN + "Server: "
							+ MAGENTA + ipToString(server.getSockAdress()) + " "
							+ YELLOW + intToString(portToInt(server.getSockAdress())) + " "
							+ RESET + "\n";
	logOut(logLevel, logEntry);
}

/**========================================================================
 *!                           DEBUG FUN!!!
*========================================================================**/
void Logger::log(logLevel logLevel, const std::string& message, const Server&server, bool yesNo)
{
	if (_logLevel[logLevel] == 0)
		return ;
	//[timestamp][loglevel][message][ip][port]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
							+ BOLD_HIGH_INTENSITY_WHITE + "Server: "
							+ MAGENTA + ipToString(server.getSockAdress()) + " "
							+ YELLOW + intToString(portToInt(server.getSockAdress())) + " "
							+ RESET + "\n";
	std::ofstream logFile("debugOutput.log", std::ios_base::app);  
	if (logFile.is_open() && yesNo)
	{
		logFile << removeAnsiCodes(logEntry);
		logFile.close();
	} else {
		std::cerr << "Erreur : impossible d'ouvrir le fichier de log." << std::endl;
	}
}

void Logger::log(logLevel logLevel, std::string& message, struct Client& client, const Error& error)
{
	if (_logLevel[logLevel] == 0)
		return ;
	// [timestamp][errorCode][loglevel][message][ip][port]
	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ GREEN + message + " "
							+ GREEN
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd) + " "
							+ RESET + "\n";
	logOut(logLevel, logEntry);
	(void)error;
}

/**========================================================================
 *                           LOG BUILDRESPONSE
 * ? changes to be made as soon as Client struct is accessible
 *========================================================================**/
// void Logger::log(	logLevel logLevel, const std::string& message,
// 					const class buildResponse& obj)
// {
// 	//[timestamp][loglevel][message][status code][ip][port][fd]
// 	std::string logEntry = 	BLUE + timeStamp::getTime() + ": " 
// 							+ formatLogLevel(logLevel) 
// 							+ GREEN + message + " "
// 							+ RED + obj.getStatusLine()
// 							// + MAGENTA + "Client: " + ipToString(obj.getClient()->address) + " "
// 							// + YELLOW + intToString(portToInt(obj.getClient()->address)) + " "
// 							// + GREEN + intToString(obj.getClient()->fd)
// 							+ RESET + "\n";
// 	_accessFile << logEntry;
// 	if (logToStdOut)
// 		std::cout << logEntry;
// 	(void)obj;
// 	_accessFile.flush();
// }

/**========================================================================
 *                       SINGLETON ACCESS
 *========================================================================**/
Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}


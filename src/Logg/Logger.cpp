#include "Logger.hpp"

/**========================================================================
 *                           CONSTRUCTOR && DESTRUCTOR
 *========================================================================**/
Logger::Logger(int flags) : logToStdOut(1),  _nl(false), _flags(flags)
{	
	_logLevel[EVAL] = _flags & L_EVL ? 1 : 0;
	_logLevel[DEBUG] = _flags & L_DEB ? 1 : 0;
	_logLevel[INFO] = _flags & L_SLN ? 0 : 1;
	_logLevel[WARNING] = 	1;
	_logLevel[ERROR] = 		1;

	_accessFile.open("access.log", std::ios::trunc);
	_errorFile.open("error.log", std::ios::trunc);
	if (!_accessFile.is_open() || !_errorFile.is_open())
		Logger::getInstance().log(ERROR, "log file");
}	

Logger::~Logger()
{
	if (_accessFile.is_open())
		_accessFile.close();
	if (_errorFile.is_open())
		_errorFile.close();
}

Logger& Logger::getInstance(int verbose)
{
	static Logger instance(verbose);
	return instance;
}

/**========================================================================
 *                           UTILS
 *========================================================================**/
std::string Logger::formatLogLevel(logLevel level) const
{
	switch (level)
	{
		case INFO:
			return std::string(GREEN) + "[INFO] " + RESET;
		case DEBUG:
			return std::string(BLUE) + "[DEBUG] " + RESET;
		case EVAL:
			return std::string(MAGENTA) + "[EVAL] " + RESET;
		case WARNING:
			return std::string(HIGH_INTENSITY_YELLOW) + "[WARNING] " + RESET;
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
	return ntohs(addr.sin_port);
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

	for (size_t i = 0; i < message.length(); ++i)
	{
		if (message[i] == '\033')
			inAnsi = true;
		else if (inAnsi && (message[i] == 'm' || message[i] == 'K'))
			inAnsi = false;
		else if (!inAnsi)
			cleanMessage += message[i];
	}
	return cleanMessage;
}

/**========================================================================
 *                           LOG OVERLOADS
 *========================================================================**/
void Logger::logOut(logLevel logLevel, const std::string& message)
{
	std::string logEntry;

	if (logToStdOut)
		std::cerr << message;
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

void Logger::log(logLevel logLevel, const std::string& message, int _logFlags)
{
	std::string logEntry;

	if ((_logLevel[logLevel] == 0 || (_logFlags & L_VRB && ~_flags & L_VRB))
		&& ~_logFlags & L_ALW)
		return ;
	_nl = true;
	logEntry =	BLUE + timeStamp::getTime() + ": " 
				+ formatLogLevel(logLevel) 
				+ GREEN + message 
				+ RESET + "\n";
				
	logOut(logLevel, logEntry);
}

void Logger::log(logLevel logLevel, const std::string& message, const RequestParser& obj, int _logFlags)
{
	std::string logEntry;

	if ((_logLevel[logLevel] == 0 || (_logFlags & L_VRB && ~_flags & L_VRB))
		&& ~_logFlags & L_ALW)
		return ;
	_nl = true;
	//[timestamp][loglevel][message][method][ip][port][fd]
	if (obj.getClient())
	{
		logEntry = 	BLUE + timeStamp::getTime() + ": " 
								+ formatLogLevel(logLevel) 
								+ GREEN + message + " "
								+ MAGENTA + "Client: " + ipToString(obj.getClient()->address) + " "
								+ HIGH_INTENSITY_YELLOW + intToString(portToInt(obj.getClient()->address)) + " "
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

void Logger::log(logLevel logLevel, const std::string& message, const Client& client, int _logFlags)
{
	std::string logEntry;

	if ((_logLevel[logLevel] == 0 || (_logFlags & L_VRB && ~_flags & L_VRB))
		&& ~_logFlags & L_ALW)
		return ;
	_nl = true;
	//[timestamp][loglevel][message][ip][port][fd]
	logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ GREEN + message + " "
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ HIGH_INTENSITY_YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd)
							+ RESET + "\n";
	logOut(logLevel, logEntry);
}

void Logger::log(logLevel logLevel, const std::string& message, const Client& client, const Server& server, int _logFlags)
{
	std::string logEntry;

	if ((_logLevel[logLevel] == 0 || (_logFlags & L_VRB && ~_flags & L_VRB))
		&& ~_logFlags & L_ALW)
		return ;
	_nl = true;
	//[timestamp][loglevel][message][ip][port][fd]
	logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) + " "
							+ GREEN + message
							+ MAGENTA + "Client: " + ipToString(client.address) + " "
							+ HIGH_INTENSITY_YELLOW + intToString(portToInt(client.address)) + " "
							+ GREEN + intToString(client.fd) + " "
							+ GREEN + "Server: "
							+ MAGENTA + ipToString(server.getSockAdress()) + " "
							+ HIGH_INTENSITY_YELLOW + intToString(portToInt(server.getSockAdress())) + " "
							+ GREEN + intToString(server._fd) + " "
							+ RESET + "\n";
	logOut(logLevel, logEntry);
}

// void Logger::log(logLevel logLevel, const std::string& message, const Client& client, bool yesNo)
// {
// 	std::string logEntry;

// 	if (_logLevel[logLevel] == 0)
// 		return ;
// 	//[timestamp][loglevel][message][ip][port][fd]
// 	logEntry = 	BLUE + timeStamp::getTime() + ": " 
// 							+ formatLogLevel(logLevel) + " "
// 							+ BOLD_HIGH_INTENSITY_WHITE + message
// 							+ MAGENTA + "Client: " + ipToString(client.address) + " "
// 							+ HIGH_INTENSITY_YELLOW + intToString(portToInt(client.address)) + " "
// 							+ GREEN + intToString(client.fd) + " "
// 							+ BOLD_HIGH_INTENSITY_WHITE + "Server: "
// 							+ RESET + "\n";
//     std::string filename = intToString(client.fd) + ".log";  
//     std::ofstream logFile(filename.c_str());      
//     if (logFile.is_open() && yesNo)
//     {
//         logFile << removeAnsiCodes(logEntry);
//         logFile.close();
//     } 
//     else 
//         std::cerr << "Erreur : impossible d'ouvrir le fichier de log." << std::endl;
// }

void Logger::log(logLevel logLevel, const std::string& message, const Server& server, int _logFlags)
{
	std::string logEntry;
	
	if ((_logLevel[logLevel] == 0 || (_logFlags & L_VRB && ~_flags & L_VRB))
		&& ~_logFlags & L_ALW)
		return ;
	_nl = true;
	//[timestamp][loglevel][message][ip][port]
	logEntry = 	BLUE + timeStamp::getTime() + ": " 
							+ formatLogLevel(logLevel) 
							+ GREEN + message + " "
							+ GREEN + "Server: "
							+ MAGENTA + ipToString(server.getSockAdress()) + " "
							+ HIGH_INTENSITY_YELLOW + intToString(portToInt(server.getSockAdress())) + " "
							+ GREEN + intToString(server._fd) + " "
							+ RESET + "\n";
	logOut(logLevel, logEntry);
}

// void Logger::log(logLevel logLevel, const std::string& message, const Server&server, bool yesNo)
// {
// 	std::string logEntry;

// 	if (_logLevel[logLevel] == 0)
// 		return ;
// 	//[timestamp][loglevel][message][ip][port]
// 	logEntry = 	BLUE + timeStamp::getTime() + ": " 
// 							+ formatLogLevel(logLevel) 
// 							+ BOLD_HIGH_INTENSITY_WHITE + message + " "
// 							+ BOLD_HIGH_INTENSITY_WHITE + "Server: "
// 							+ MAGENTA + ipToString(server.getSockAdress()) + " "
// 							+ HIGH_INTENSITY_YELLOW + intToString(portToInt(server.getSockAdress())) + " "
// 							+ RESET + "\n";
// 	std::ofstream logFile("debugOutput.log", std::ios_base::app);  
// 	if (logFile.is_open() && yesNo)
// 	{
// 		logFile << removeAnsiCodes(logEntry);
// 		logFile.close();
// 	}
// 	else
// 		std::cerr << "Erreur : impossible d'ouvrir le fichier de log." << std::endl;
// }

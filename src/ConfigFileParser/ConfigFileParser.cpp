#include "ConfigFileParser.hpp"

/**========================================================================
 *                           CONSTRUCTOR
 *========================================================================**/
ConfigFileParser::ConfigFileParser()
{
	routeKey.push_back("allowedMethods"); 
	routeKey.push_back("redirection");
	routeKey.push_back("uri");
	routeKey.push_back("root");
	routeKey.push_back("listingDirectory");
	routeKey.push_back("index");
	routeKey.push_back("cgiAllowed");
	routeKey.push_back("uploadAllowed");
	routeKey.push_back("uploadDirectory");
}

/**========================================================================
 *                           MAIN FUNCS
 *========================================================================**/
void ConfigFileParser::parseConfigFile(Config& configStruct, char* path)
{
	extractDataFromConfigFile(path);
	intializeConfigStruct(configStruct);
	assignRoutesToServers(configStruct);
}

void	ConfigFileParser::intializeConfigStruct(Config& configStruct)
{
	int	i = 0;
	for (catIt catIt = _data.begin(); catIt != _data.end(); ++catIt)
	{
		if (!configStruct._serverStruct[i].port.empty())
			i++;
		for (itemIt itemIt = catIt->second.begin(); itemIt != catIt->second.end(); ++itemIt)
			for (valIt valIt = itemIt->second.begin(); valIt != itemIt->second.end(); ++valIt)
				setAllConfigValues(catIt, itemIt, valIt, configStruct, i);
	}
}

void ConfigFileParser::setAllConfigValues(catIt& catIt, itemIt& itemIt, valIt& valIt, Config& configStruct, int& i)
{
	// Category "global"
	setConfigValue(catIt, itemIt, configStruct.maxClient, "maxClient");
	setConfigValue(catIt, itemIt, configStruct.maxHeaderSize, "maxHeaderSize");
	setConfigValue(catIt, itemIt, configStruct.maxServerNbr, "maxServerNbr");
	setConfigValue(catIt, itemIt, configStruct.maxBodySize, "maxBodySize");
	setConfigValue(catIt, itemIt, configStruct.recv_buff_size, "recv_buff_size");
	setConfigValue(catIt, itemIt, configStruct.send_buff_size, "send_buff_size");
	setConfigValue(catIt, itemIt, configStruct.timeoutCgi, "timeoutCgi");
	setConfigValue(catIt, itemIt, configStruct.listingDirectories, "listingDirectories");
	setConfigValue(catIt, itemIt, configStruct.handleCookies, "handleCookies");
	setConfigValue(itemIt, valIt, configStruct.indexFiles, "indexFiles");
	// category "errorPages"
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_201", CODE_201_CREATED);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_400", CODE_400_BAD_REQUEST);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_401", CODE_401_UNAUTHORIZED);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_403", CODE_403_FORBIDDEN);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_404", CODE_404_NOT_FOUND);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_405", CODE_405_METHOD_NOT_ALLOWED);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_413", CODE_413_PAYLOAD_TOO_LARGE);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_422", CODE_422_UNPROCESSABLE_ENTITY);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_431", CODE_431_REQUEST_HEADER_FIELDS_TOO_LARGE);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_500", CODE_500_INTERNAL_SERVER_ERROR);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_502", CODE_502_BAD_GATEWAY);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_503", CODE_503_SERVICE_UNAVAILABLE);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_504", CODE_504_GATEWAY_TIMEOUT);
	setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_508", CODE_508_LOOP_DETECTED);
	// category server
	setConfigValue(catIt, itemIt, valIt, configStruct._serverStruct[i].host, "host");
	setConfigValue(catIt, itemIt, valIt, configStruct._serverStruct[i].port, "port");
	setConfigValue(catIt, configStruct._serverStruct[i]);
	setConfigValue(catIt, itemIt, valIt, configStruct._serverStruct[i].serverName, "serverName");
	setConfigValue(itemIt, valIt, configStruct._serverStruct[i].allowedRoutes, "allowedRoutes");
	// category routes (inner loop)
	setConfigValue(catIt, itemIt, valIt, configStruct);
}

void ConfigFileParser::assignRoutesToServers(Config& configStruct)
{
	for (int i = 0; i < 4; ++i)
	{
		for (unsigned long j = 0;	j != configStruct._serverStruct[i].allowedRoutes.size(); j++)
		{
			std::string routeName = configStruct._serverStruct[i].allowedRoutes[j];
			RoutesData::const_iterator routeIt = configStruct.routes.find(routeName);
			if (routeIt != configStruct.routes.end())
				configStruct._serverStruct[i].routesData[routeName] = routeIt->second;
		}
	}
}

void	ConfigFileParser::initializeServers(Config& configStruct, int& i)
{
	for (int j = 0; j < i + 1; j++)
	{
		struct sockaddr_in server;	
		std::memset(&server, 0, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(_serverStruct[j].host.c_str());
		server.sin_port = htons(static_cast<uint16_t>(std::atoi(_serverStruct[j].port.c_str())));
		configStruct.sockAddress.push_back(server);
		std::cout << "server " << _serverStruct[j].serverName << " initialized " << std::endl;
	}
}

/**========================================================================
 *                           SETCONFIGVALUE OVERLOADS
 *========================================================================**/
//? routes data
void	ConfigFileParser::setConfigValue(catIt catIt, itemIt itemIt, valIt valIt, Config& configStruct)
{
	for (size_t i = 0; i < routeKey.size(); i++)
	{
		if (isRouteData(catIt->first) && itemIt->first == routeKey[i])
			if (!(*valIt).empty())
				configStruct.routes[catIt->first][routeKey[i]] = itemIt->second;
	}
}

//? error pages paths
void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, Config& configStruct, const char str[], e_errorCodes e)
{
	if (catIt->first == "errorPages" && itemIt->first == str)
		if (!(*valIt).empty())
			configStruct.errorPaths[e] = configStruct.errorPagesPath + *valIt;
}

//? struct server
void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, std::string& field, const char str[])
{
	if (isServerData(catIt->first) && itemIt->first == str)
	{
		if (itemIt->first == "port")
		{
			int portNbr = std::atoi(itemIt->second[0].c_str());
			if (portNbr > 65535 || portNbr < 1024)
			{
				Logger::getInstance().log(WARNING, "Config file error: wrong port value");
				return;
			}
		}
		if (!(*valIt).empty())
			field = itemIt->second[0];		
	}
}

void	ConfigFileParser::setConfigValue(catIt& catIt, server& serverStruct)
{
	serverStruct.serverStructName = catIt->first;
}

//? maxBodySize
void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, size_t& field, const char str[])
{
	if (catIt->first == "global" && itemIt->first == str)
		if (!itemIt->second[0].empty())
		{
			field = (size_t)std::atoi(itemIt->second[0].c_str());
			if (field > INT64_MAX && itemIt->first == "maxBodySize")
			{
				Logger::getInstance().log(WARNING, "Config file error: wrong maxBodySize value");
				field = 0;
			}
			if (field > INT64_MAX && (itemIt->first == "send_buff_size" || itemIt->first == "recv_buff_size"))
			{
				Logger::getInstance().log(WARNING, "Config file error: wrong buffSize value");
				field = 0;
			}
		}
}

//? maxHederSize
void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, ssize_t& field, const char str[])
{
	if (catIt->first == "global" && itemIt->first == str)
		if (!itemIt->second[0].empty())
			field = (ssize_t)std::atoi(itemIt->second[0].c_str());
	if ((field < 0 || field > 8192) && itemIt->first == "maxHeaderSize")
	{
		Logger::getInstance().log(WARNING, "Config file error: wrong maxHeaderSize value");
		field = 0;
	}
}

//? maxClient
void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, short& field, const char str[])
{
	if (catIt->first == "global" && itemIt->first == str)
		if (!itemIt->second[0].empty())
			field = (short)std::atoi(itemIt->second[0].c_str());
	if (field < 0)
	{
		Logger::getInstance().log(WARNING, "Config file error: wrong MaxClient value");
		field = 0;
	}
}

//? recv_buff_size/send_buff_size
void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, unsigned short& field, const char str[])
{
	if (catIt->first == "global" && itemIt->first == str)
		if (!itemIt->second[0].empty())
		{
			int tmp = (int)std::atoi(itemIt->second[0].c_str());
			if(tmp >= 0 && tmp < 65535)
				field = (unsigned short)tmp;
			else
			{
				Logger::getInstance().log(WARNING, "Config file error: wrong buff_size value");
				field = 0;
			}
		}
}

//? maxServerNbr
void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, int& field, const char str[])
{
	if (itemIt->first == "maxServerNbr")
		field = 0;

	if (catIt->first == "global" && itemIt->first == str)
		if (!itemIt->second[0].empty())
			field = (int)std::atoi(itemIt->second[0].c_str());
	if  ((field < 0 || field > 8) && itemIt->first == "maxServerNbr")
	{
		Logger::getInstance().log(WARNING, "Config file error: wrong Server value");
		throw std::exception();
	}
	if  ((field < 0 || field > 1000) && itemIt->first == "timeoutCgi")
	{
		Logger::getInstance().log(WARNING, "Config file error: wrong timeoutCgi value");
		field = 10;
	}
}

//? listingDirectories && handleCookies
void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, bool& field, const char str[])
{
	if (catIt->first == "errorPages")
		return ;
	if (catIt->first == "global" && itemIt->first == str)
		if (!itemIt->second[0].empty())
			field = std::atoi(itemIt->second[0].c_str());
	if  ((field != 0 && field != 1)  && itemIt->first == "listingDirectories")
	{
		Logger::getInstance().log(WARNING, "Config file error: wrong listingDirectories value");
		field = 0;
	}
	if  ((field != 0 && field != 1)  && itemIt->first == "handleCookies")
	{
		Logger::getInstance().log(WARNING, "Config file error: wrong handleCookies value");
		field = 0;
	}
}

//? indexFiles
void	ConfigFileParser::setConfigValue(itemIt& itemIt, valIt& valIt, std::vector<std::string>& vec, const char str[])
{
	static bool hasUserConfig = false;
	if (itemIt->first == str)
		if (!(*valIt).empty())
		{
			if (!hasUserConfig)
			{
				vec.clear();
				hasUserConfig = true;
			}
			vec.push_back(*valIt);
		}
}

/**========================================================================
 *                           EXTRACTION FROM FILE
 *========================================================================**/
void	ConfigFileParser::extractKeyValuePairs(std::string& line, std::string& currentCategory)
{
	size_t colonPos = line.find('=');
	if (colonPos != std::string::npos)
	{
		std::string key = line.substr(0, colonPos);
		std::string value = line.substr(colonPos + 1);
		RequestParser::trim(key); RequestParser::trim(value);
		std::istringstream valueStream(value);
		std::string singleValue;
		while (std::getline(valueStream, singleValue, ','))
		{
			RequestParser::trim(singleValue);
			_data[currentCategory][key].push_back(singleValue);
		}
	}
}

int	ConfigFileParser::extractDataFromConfigFile(const std::string& path)
{
	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		Logger::getInstance().log(WARNING, "could not open config file. Starting Webserv with default file...");
		file.open("_configs/base_test/base_test_ok_1.ini");
	}
	if (!file.is_open())
	{
		Logger::getInstance().log(ERROR, "could not open config file. Stop dun us...");
		throw std::exception();
	}
	std::string line;
	std::string currentCategory;
	while (getline(file, line))
	{
		if (ignoreComents(line) == 0)
			continue ;
		if(getCurrentCategory(line, currentCategory) == 0)
			continue ;
		extractKeyValuePairs(line, currentCategory);
	}
	file.close();
	return (0);
}

/**========================================================================
 *                           UTILS
 *========================================================================**/
int	ConfigFileParser::ignoreComents(std::string& line)
{
	RequestParser::trim(line);
	size_t firstChar = line.find('#');
	if (firstChar != std::string::npos)
		line.erase(firstChar);
	if (line.empty())
		return (0);
	firstChar = line.find_first_not_of(" \t");
	if (firstChar != std::string::npos)
		line.erase(0, firstChar);
	if (line.empty())
		return (0);
	firstChar = line.find("]");
	if (firstChar != std::string::npos)
		line.erase(firstChar + 1);
	if (line.find_first_not_of(" \t") == std::string::npos)
		return (0);
	return (1);
}

int	ConfigFileParser::getCurrentCategory(std::string& line, std::string& currentCategory)
{
	RequestParser::trim(line);
	if (!line.empty() && line[0] == '[')
	{
		size_t lastChar = line.find_last_not_of(" \t");
		if (lastChar != std::string::npos && line[lastChar] == ']')
		{
			currentCategory = line.substr(1, lastChar - 1);
			if (_data.find(currentCategory) == _data.end())
				_data[currentCategory] = std::map<std::string, std::vector<std::string> >();
			return (0);
		}
	}
	return (1);
}

bool ConfigFileParser::isServerData(const std::string& category)
{
	const std::string prefix = "server";
	return category.size() >= prefix.size() && category.find(prefix) == 0;
}

bool ConfigFileParser::isRouteData(const std::string& category)
{
	const std::string prefix = "route";
	return category.size() >= prefix.size() && category.find(prefix) == 0;
}

bool ConfigFileParser::isAllowedRoute(const std::string& str, server& serverStruct)
{
	std::vector<std::string>::iterator it;
	it = std::find(serverStruct.allowedRoutes.begin(), serverStruct.allowedRoutes.end(), str);
	return it != serverStruct.allowedRoutes.end();
}

/**========================================================================
 *                           DISPLAY
 *========================================================================**/
void ConfigFileParser::printData(const std::map<std::string, std::map<std::string, std::vector<std::string> > >& _data)
{
	for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator catIt = _data.begin(); catIt != _data.end(); ++catIt)
	{
		std::cout << "Category: " << catIt->first << std::endl;
		for (std::map<std::string, std::vector<std::string> >::const_iterator itemIt = catIt->second.begin(); itemIt != catIt->second.end(); ++itemIt)
		{
			std::cout << "  Key: >" << itemIt->first +"<" << std::endl;
			std::cout << "  Values: >";
			for (std::vector<std::string>::const_iterator valIt = itemIt->second.begin(); valIt != itemIt->second.end(); ++valIt)
				std::cout << *valIt << "< >"; 
			std::cout << std::endl;
		}
	}
}

void ConfigFileParser::printServerData(const server _serverStruct[], size_t size)
{
	for (size_t i = 0; i < size; ++i) {
		if (!_serverStruct[i].host.empty())
		{
			std::cout << "Server " << i + 1 << ":" << std::endl;
			std::cout << "  Host: " << _serverStruct[i].host << std::endl;
			std::cout << "  Port: " << _serverStruct[i].port << std::endl;
			std::cout << "  Server Name: " << _serverStruct[i].serverName << std::endl;
			std::cout << "  ServerStruct Name: " << _serverStruct[i].serverStructName << std::endl;
			std::cout << "  Allowed Routes: ";
			for(size_t j = 0; j < _serverStruct[i].allowedRoutes.size(); j++)
				std::cout << _serverStruct[i].allowedRoutes[j] << " ";
			std::cout << std::endl;
			printRoutesData(_serverStruct[i].routesData);
			std::cout << "\n------------------------" << std::endl;
		}
	}
}

void ConfigFileParser::printConfig(const Config& config)
{
	std::cout << "maxClient: " << config.maxClient << std::endl;
	std::cout << "recv_buff_size: " << config.recv_buff_size << std::endl;
	std::cout << "send_buff_size: " << config.send_buff_size << std::endl;

	std::cout << "sockAddress:" << std::endl;
	for (std::vector<struct sockaddr_in>::const_iterator it = config.sockAddress.begin(); it != config.sockAddress.end(); ++it) {
		std::cout << "  - IP: " << inet_ntoa(it->sin_addr) << ", Port: " << ntohs(it->sin_port) << std::endl;
	}

	std::cout << "indexFiles: ";
	for (std::vector<std::string>::const_iterator it = config.indexFiles.begin(); it != config.indexFiles.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::cout << "listingDirectories: " << (config.listingDirectories ? "true" : "false") << std::endl;

	std::cout << "errorPaths:" << std::endl;
	for (std::map<e_errorCodes, std::string>::const_iterator it = config.errorPaths.begin(); it != config.errorPaths.end(); ++it) {
		std::cout << "  Code: " << it->first << " -> Path: " << it->second << std::endl;
	}
}

void ConfigFileParser::printRoutesData(const RoutesData& routesData)
{
	std::cout << "ROUTES:" << std::endl;
	for (RoutesData::const_iterator routeIt = routesData.begin(); routeIt != routesData.end(); ++routeIt) {
		std::cout << "Route: " << routeIt->first << std::endl;
		for (std::map<std::string, std::vector<std::string> >::const_iterator methodIt = routeIt->second.begin(); 
			methodIt != routeIt->second.end(); ++methodIt)
			{
			std::cout << "  Key: " << methodIt->first << std::endl;
			std::cout << "    Value/Values: ";
			for (std::vector<std::string>::const_iterator paramIt = methodIt->second.begin(); 
				paramIt != methodIt->second.end(); ++paramIt)
				{
				std::cout << *paramIt << " ";
			}
			std::cout << std::endl;
		}
	}
}

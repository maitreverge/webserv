#include "ConfigFileParser.hpp"
#include "tmpConfig.hpp"
#include <cstdlib>

bool isServerData(const std::string& category)
{
	const std::string prefix = "server";
	return category.size() >= prefix.size() && category.find(prefix) == 0;
}

void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, Config& configStruct, const char str[], e_errorCodes e)
{
	if (catIt->first == "errorPages" && itemIt->first == str)
		if (!(*valIt).empty())
			configStruct.errorPaths.insert(std::make_pair(e, itemIt->second[0]));
}

void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, std::string& field, const char str[], int& i)
{
	if (isServerData(catIt->first) && itemIt->first == str)
		if (!(*valIt).empty())
			field = itemIt->second[0];
}

void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, short& field, const char str[])
{
	if (catIt->first == "global" && itemIt->first == str)
		if (!itemIt->second[0].empty())
			field = std::atoi(itemIt->second[0].c_str());
}

void	ConfigFileParser::setConfigValue(catIt& catIt, itemIt& itemIt, bool& field, const char str[])
{
	if (catIt->first == "global" && itemIt->first == str)
		if (!itemIt->second[0].empty())
			field = std::atoi(itemIt->second[0].c_str());
}

void	ConfigFileParser::intializeConfigStruct(Config configStruct)
{
	int	i = 0;
	for (catIt catIt = _data.begin(); catIt != _data.end(); ++catIt)
	{
		if (!_serverStruct[i].host.empty() && !_serverStruct[i].port.empty())
			i++;
		for (itemIt itemIt = catIt->second.begin(); itemIt != catIt->second.end(); ++itemIt)
		{
			for (valIt valIt = itemIt->second.begin(); valIt != itemIt->second.end(); ++valIt)
			{
				// Category "global"
				setConfigValue(catIt, itemIt, configStruct.maxClient, "maxClient");
				setConfigValue(catIt, itemIt, configStruct.listingDirectories, "listingDirectories");
				
				if (catIt->first == "global" && itemIt->first == "indexFiles")
					if (!(*valIt).empty())
						configStruct.indexFiles.push_back(*valIt);
				// category "errorPages"
				setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_400", CODE_400_BAD_REQUEST);
				setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_401", CODE_401_UNAUTHORIZED);
				setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_403", CODE_403_FORBIDDEN);
				setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_404", CODE_404_NOT_FOUND);
				setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_500", CODE_500_INTERNAL_SERVER_ERROR);
				setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_502", CODE_502_BAD_GATEWAY);
				setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_503", CODE_503_SERVICE_UNAVAILABLE);
				setConfigValue(catIt, itemIt, valIt, configStruct, "errorPage_504", CODE_504_GATEWAY_TIMEOUT);
				// category server
				setConfigValue(catIt, itemIt, valIt, _serverStruct[i].host, "host", i);
				setConfigValue(catIt, itemIt, valIt, _serverStruct[i].port, "port", i);
				setConfigValue(catIt, itemIt, valIt, _serverStruct[i].serverName, "serverName", i);
			}
		}
	}
	printData(_data);
	printServerData(_serverStruct, i + 1);
}

/**================================================================================================
 *?                                          WHAT NEXT?
*? problemes potentiels pour l'initialisation des serveurs:
*? - les serveurs eux-memes ne sont pas encore initialises avec ces donnees
*? - une fois initialises, les serveurs doivent etre ajoutes au std::vector
*? - grand besoin de refactorisation une fois que la logique fonctionnera.
*================================================================================================**/

int main(void)
{
	Config configStruct;
	ConfigFileParser toto;
	toto.extractDataFromConfigFile("config.ini");
	toto.intializeConfigStruct(configStruct);

	return (0);
}


void	ConfigFileParser::print(std::string str)
{
	std::cout << str << std::endl;
}

void	ConfigFileParser::trim(std::string& str)
{
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
}

void ConfigFileParser::printData(const std::map<std::string, std::map<std::string, std::vector<std::string> > >& _data) {
	for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator catIt = _data.begin(); catIt != _data.end(); ++catIt)
	{
		std::cout << "Category: " << catIt->first << std::endl;
		for (std::map<std::string, std::vector<std::string> >::const_iterator itemIt = catIt->second.begin(); itemIt != catIt->second.end(); ++itemIt)
		{
			std::cout << "  Key: >" << itemIt->first +"<" << std::endl;
			std::cout << "  Values: >";
			for (std::vector<std::string>::const_iterator valIt = itemIt->second.begin(); valIt != itemIt->second.end(); ++valIt)
			{
				std::cout << *valIt << "< >"; 
			}
			std::cout << std::endl;
		}
	}
}

void ConfigFileParser::printServerData(const server _serverStruct[], size_t size) {
	for (size_t i = 0; i < size; ++i) {
		std::cout << "Server " << i + 1 << ":" << std::endl;
		std::cout << "  Host: " << _serverStruct[i].host << std::endl;
		std::cout << "  Port: " << _serverStruct[i].port << std::endl;
		std::cout << "  Server Name: " << _serverStruct[i].serverName << std::endl;
		std::cout << "------------------------" << std::endl;
	}
}

int	ConfigFileParser::ignoreComents(std::string& line)
{
	size_t firstChar = line.find('#');
	if (firstChar != std::string::npos)
		line.erase(firstChar);
	firstChar = line.find_first_not_of(" \t");
	if (firstChar != std::string::npos)
		line.erase(0, firstChar);
	firstChar = line.find("]");
	if (firstChar != std::string::npos)
		line.erase(firstChar + 1);
	if (line.find_first_not_of(" \t") == std::string::npos)
		return (0);
	return (1);
}

int	ConfigFileParser::getCurrentCategory(std::string& line, std::string& currentCategory)
{
	if (!line.empty() && line[0] == '[')
	{
		size_t lastChar = line.find_last_not_of(" \t");
		if (lastChar != std::string::npos && line[lastChar] == ']')
		{
			currentCategory = line.substr(1, lastChar - 1);
			// print("\nCurrent category: " + currentCategory);
			if (_data.find(currentCategory) == _data.end()) {
				_data[currentCategory] = std::map<std::string, std::vector<std::string> >();
				// print("Added new category: " + currentCategory);
			}
			return (0);
		}
	}
	// print(currentCategory + " =>	" + line);
	return (1);
}

void	ConfigFileParser::extractKeyValuePairs(std::string& line, std::string& currentCategory)
{
	size_t colonPos = line.find('=');
	if (colonPos != std::string::npos)
	{
		std::string key = line.substr(0, colonPos);
		std::string value = line.substr(colonPos + 1);
		trim(key); trim(value);
		std::istringstream valueStream(value);
		std::string singleValue;
		while (std::getline(valueStream, singleValue, ','))
		{
			trim(singleValue);
			_data[currentCategory][key].push_back(singleValue);
		}
	}
}

int	ConfigFileParser::extractDataFromConfigFile(const std::string str)
{
	std::ifstream file(str.c_str());
	if (!file.is_open())
		return (std::cerr << "could not open config file" << std::endl, 1);
	// print("Config file opened");
	std::string line;
	std::string currentCategory;
	while (getline(file, line))
	{
		// ignore comments
		if (ignoreComents(line) == 0)
			continue ;
		// get current category
		if(getCurrentCategory(line, currentCategory) == 0)
			continue ;
		// add key-value(s) to category
		extractKeyValuePairs(line, currentCategory);
	}
	file.close();
	// print("Config file closed");
	return (0);
}
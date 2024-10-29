#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <cstdlib>
#include "Server.hpp"
#include <iostream>
#include <netinet/in.h> // pour struct sockaddr_in et AF_INET
#include <arpa/inet.h> // pour inet_ntoa
#include <cstring> // pour memset

typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator catIt;
typedef std::map<std::string, std::vector<std::string> >::const_iterator itemIt;
typedef std::vector<std::string>::const_iterator valIt;
typedef std::map<std::string, std::map<std::string, std::vector<std::string> > > Data;
struct server
{
	std::string	host;
	std::string	port;
	std::string	serverName;
};

class ConfigFileParser
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif
		struct server	_serverStruct[4];
		Data			_data;

		void	print(std::string str);
		void	trim(std::string& str);
		void	printData(const std::map<std::string, std::map<std::string, std::vector<std::string> > >& data);
		int		ignoreComents(std::string& line);
		int		getCurrentCategory(std::string& line, std::string& currentCategory);
		void	extractKeyValuePairs(std::string& line, std::string& currentCategory);
		void	printServerData(const server serverStruct[], size_t size);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, Config& configStruct, const char str[], e_errorCodes e);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, std::string& field, const char str[]);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, short& field, const char str[]);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, bool& field, const char str[]);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, std::vector<std::string> vec, const char str[]);
		void	initializeServers(Config configStruct, int& i);
		bool	isServerData(const std::string& category);

	public:
		void	intializeConfigStruct(Config configStruct);
		int		extractDataFromConfigFile(const std::string);
		void	parseConfigFile(Config configStruct, char* path);
		void 	printConfig(const Config& config);

};
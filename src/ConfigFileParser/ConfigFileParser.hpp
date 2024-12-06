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
#include "Logger.hpp"
#include "RequestParser.hpp"
#include "master.hpp"

typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator catIt;
typedef std::map<std::string, std::vector<std::string> >::const_iterator itemIt;
typedef std::vector<std::string>::const_iterator valIt;
typedef std::map<std::string, std::map<std::string, std::vector<std::string> > > Data;

class ConfigFileParser
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif
		struct server	_serverStruct[4];
		Data			_data;
		std::vector<std::string> routeKey;

		void	printData(const std::map<std::string, std::map<std::string, std::vector<std::string> > >& data);
		int		ignoreComents(std::string& line);
		int		getCurrentCategory(std::string& line, std::string& currentCategory);
		void	extractKeyValuePairs(std::string& line, std::string& currentCategory);
		
		void	setAllConfigValues(catIt& catIt, itemIt& itemIt, valIt& valIt, Config& configStruct, int& i);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, Config& configStruct, const char str[], e_errorCodes e);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, valIt& valIt, std::string& field, const char str[]);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, short& field, const char str[]);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, size_t& field, const char str[]);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, ssize_t& field, const char str[]);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, bool& field, const char str[]);
		void	setConfigValue(itemIt& itemIt, valIt& valIt, std::vector<std::string>& vec, const char str[]);
		void	setConfigValue(catIt catIt, itemIt itemIt, valIt valIt, Config& configStruct);
		void	setConfigValue(catIt& catIt, server& serverStruct);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, int& field, const char str[]);
		void	setConfigValue(catIt& catIt, itemIt& itemIt, unsigned short& field, const char str[]);
		void	initializeServers(Config& configStruct, int& i);
		bool	isServerData(const std::string& category);
		bool	isRouteData(const std::string& category);
		bool	isAllowedRoute(const std::string& str, server& serverStruct);
		void	assignRoutesToServers(Config& configStruct);
		void	intializeConfigStruct(Config& configStruct);
		int		extractDataFromConfigFile(const std::string& path);

		static void printServerData(const server _serverStruct[], size_t size);
		static void printConfig(const Config& config);
		static void	printRoutesData(const RoutesData& routesData);
		
		public:
		ConfigFileParser();
		void		parseConfigFile(Config& configStruct, char* path);

};
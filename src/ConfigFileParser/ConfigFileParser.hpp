#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "tmpConfig.hpp"

struct server
{
	std::string	host;
	std::string	port;
	std::string	serverName;
};

typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator catIt;
typedef std::map<std::string, std::vector<std::string> >::const_iterator itemIt;
typedef std::vector<std::string>::const_iterator valIt;
class ConfigFileParser
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif
		struct server _serverStruct[4];
		std::map<std::string, std::map<std::string, std::vector<std::string> > > _data;
		std::map<std::string, std::map<std::string, std::vector<std::string> > > data;
	public:

	void	print(std::string str);
	void	trim(std::string& str);
	void	printData(const std::map<std::string, std::map<std::string, std::vector<std::string> > >& data);
	int		ignoreComents(std::string& line);
	int		getCurrentCategory(std::string& line, std::string& currentCategory);
	void	extractKeyValuePairs(std::string& line, std::string& currentCategory);
	int		extractDataFromConfigFile(const std::string);
	void	intializeConfigStruct(Config configStruct);
	void	printServerData(const server serverStruct[], size_t size);
};
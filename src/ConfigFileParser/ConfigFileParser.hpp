#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "tmpConfig.hpp"

class ConfigFileParser
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif
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
		
};
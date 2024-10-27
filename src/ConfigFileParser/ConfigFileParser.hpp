#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

class ConfigFileParser
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif
		std::map<std::string, std::map<std::string, std::vector<std::string> > > data;
	public:
		
};
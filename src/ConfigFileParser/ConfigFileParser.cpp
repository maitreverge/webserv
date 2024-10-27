#include "ConfigFileParser.hpp"
#include "tmpConfig.hpp"
#include <cstdlib>

using namespace std;
map<string, map<string, vector<string> > > data;

void	ConfigFileParser::intializeConfigStruct(Config configStruct)
{
	// printData(data);
	for (map<string, map<string, vector<string> > >::const_iterator catIt = data.begin(); catIt != data.end(); ++catIt)
	{
		cout << "Category: " << catIt->first << endl;
		for (map<string, vector<string> >::const_iterator itemIt = catIt->second.begin(); itemIt != catIt->second.end(); ++itemIt)
		{
			cout << "  Key: >" << itemIt->first +"<" << endl;
			cout << "  Values: >";
			for (vector<string>::const_iterator valueIt = itemIt->second.begin(); valueIt != itemIt->second.end(); ++valueIt)
			{
				// Category "global"
				if (catIt->first == "global" && itemIt->first == "maxClient")
					if (!itemIt->second[0].empty())
					{
						configStruct.maxClient = std::atoi(itemIt->second[0].c_str());
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "global" && itemIt->first == "listingDirectories")
					if (!itemIt->second[0].empty())
					{
						configStruct.listingDirectories = std::atoi(itemIt->second[0].c_str());
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "global" && itemIt->first == "indexFiles")
					if (!(*valueIt).empty())
					{
						configStruct.indexFiles.push_back(*valueIt);
						print("*******" + *valueIt + " added to struct *******");
					}
				// category "errorPages"
				if (catIt->first == "errorPages" && itemIt->first == "errorPage_400")
					if (!(*valueIt).empty())
					{
						configStruct.errorPaths.insert(std::make_pair(CODE_400_BAD_REQUEST, itemIt->second[0]));
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "errorPages" && itemIt->first == "errorPage_401")
					if (!(*valueIt).empty())
					{
						configStruct.errorPaths.insert(std::make_pair(CODE_401_UNAUTHORIZED, itemIt->second[0]));
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "errorPages" && itemIt->first == "errorPage_403")
					if (!(*valueIt).empty())
					{
						configStruct.errorPaths.insert(std::make_pair(CODE_403_FORBIDDEN, itemIt->second[0]));
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "errorPages" && itemIt->first == "errorPage_404")
					if (!(*valueIt).empty())
					{
						configStruct.errorPaths.insert(std::make_pair(CODE_404_NOT_FOUND, itemIt->second[0]));
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "errorPages" && itemIt->first == "errorPage_500")
					if (!(*valueIt).empty())
					{
						configStruct.errorPaths.insert(std::make_pair(CODE_500_INTERNAL_SERVER_ERROR, itemIt->second[0]));
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "errorPages" && itemIt->first == "errorPage_502")
					if (!(*valueIt).empty())
					{
						configStruct.errorPaths.insert(std::make_pair(CODE_502_BAD_GATEWAY, itemIt->second[0]));
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "errorPages" && itemIt->first == "errorPage_503")
					if (!(*valueIt).empty())
					{
						configStruct.errorPaths.insert(std::make_pair(CODE_503_SERVICE_UNAVAILABLE, itemIt->second[0]));
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				if (catIt->first == "errorPages" && itemIt->first == "errorPage_504")
					if (!(*valueIt).empty())
					{
						configStruct.errorPaths.insert(std::make_pair(CODE_504_GATEWAY_TIMEOUT, itemIt->second[0]));
						print("*******" + itemIt->second[0] + " added to struct *******");
					}
				// category "serverX"
				// category "routeX"
				cout << *valueIt << "< >"; 
			}
			cout << endl;
		}
	}
}
int main(void)
{
	Config configStruct;
	ConfigFileParser toto;
	toto.extractDataFromConfigFile("config.ini");
	toto.intializeConfigStruct(configStruct);
	return (0);
}


void	ConfigFileParser::print(string str)
{
	cout << str << endl;
}

void	ConfigFileParser::trim(string& str)
{
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
}

void ConfigFileParser::printData(const map<string, map<string, vector<string> > >& data) {
	for (map<string, map<string, vector<string> > >::const_iterator catIt = data.begin(); catIt != data.end(); ++catIt)
	{
		cout << "Category: " << catIt->first << endl;
		for (map<string, vector<string> >::const_iterator itemIt = catIt->second.begin(); itemIt != catIt->second.end(); ++itemIt)
		{
			cout << "  Key: >" << itemIt->first +"<" << endl;
			cout << "  Values: >";
			for (vector<string>::const_iterator valueIt = itemIt->second.begin(); valueIt != itemIt->second.end(); ++valueIt)
			{
				cout << *valueIt << "< >"; 
			}
			cout << endl;
		}
	}
}
int	ConfigFileParser::ignoreComents(string& line)
{
	size_t firstChar = line.find('#');
	if (firstChar != string::npos)
		line.erase(firstChar);
	firstChar = line.find_first_not_of(" \t");
	if (firstChar != string::npos)
		line.erase(0, firstChar);
	firstChar = line.find("]");
	if (firstChar != string::npos)
		line.erase(firstChar + 1);
	if (line.find_first_not_of(" \t") == string::npos)
		return (0);
	return (1);
}

int	ConfigFileParser::getCurrentCategory(string& line, string& currentCategory)
{
	if (!line.empty() && line[0] == '[')
	{
		size_t lastChar = line.find_last_not_of(" \t");
		if (lastChar != string::npos && line[lastChar] == ']')
		{
			currentCategory = line.substr(1, lastChar - 1);
			// print("\nCurrent category: " + currentCategory);
			if (data.find(currentCategory) == data.end()) {
				data[currentCategory] = map<string, vector<string> >();
				// print("Added new category: " + currentCategory);
			}
			return (0);
		}
	}
	// print(currentCategory + " =>	" + line);
	return (1);
}

void	ConfigFileParser::extractKeyValuePairs(string& line, string& currentCategory)
{
	size_t colonPos = line.find('=');
	if (colonPos != string::npos)
	{
		string key = line.substr(0, colonPos);
		string value = line.substr(colonPos + 1);
		trim(key); trim(value);
		istringstream valueStream(value);
		string singleValue;
		while (getline(valueStream, singleValue, ','))
		{
			trim(singleValue);
			data[currentCategory][key].push_back(singleValue);
		}
	}
}

int	ConfigFileParser::extractDataFromConfigFile(const std::string str)
{
	ifstream file(str.c_str());
	if (!file.is_open())
		return (cerr << "could not open config file" << endl, 1);
	// print("Config file opened");
	string line;
	string currentCategory;
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
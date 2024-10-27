#include "ConfigFileParser.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

void	print(std::string str)
{
	std::cout << str << std::endl;
}

int	ignoreComents(std::string& line)
{
	size_t firstChar = line.find('#');
	if (firstChar != std::string::npos)
		line.erase(firstChar);
	firstChar = line.find_first_not_of(" \t");
	if (firstChar != std::string::npos)
		line.erase(0, firstChar);
	if (line.find_first_not_of(" \t") == std::string::npos)
		return (0);
	return (1);
}

int main(void)
{
	std::ifstream file("config.ini");
	if (!file.is_open())
		return (std::cerr << "could not open config file" << std::endl, 1);
	print("Hello world");
	std::string line;
	std::string currentCategory;
	while (std::getline(file, line))
	{
		// ignore comments
		if (ignoreComents(line) == 0)
			continue;
		if (!line.empty() && line[0] == '[')
		{
			size_t lastChar = line.find_last_not_of(" \t");
			if (lastChar != std::string::npos && line[lastChar] == ']')
			{
				currentCategory = line.substr(1, lastChar - 1);
				print("\nCurrent category: " + currentCategory);
				continue;
			}
		}
		print(line);
	}
	file.close();

	return (0);
}
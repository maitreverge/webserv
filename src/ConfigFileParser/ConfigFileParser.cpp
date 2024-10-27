#include "ConfigFileParser.hpp"

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
	firstChar = line.find("]");
	if (firstChar != std::string::npos)
		line.erase(firstChar + 1);
	if (line.find_first_not_of(" \t") == std::string::npos)
		return (0);
	return (1);
}

int main(void)
{
	std::ifstream file("config.ini");
	if (!file.is_open())
		return (std::cerr << "could not open config file" << std::endl, 1);
	print("Config file opened");
	std::string line;
	std::string currentCategory;
	while (std::getline(file, line))
	{
		// ignore comments
		if (ignoreComents(line) == 0)
			continue;

		// identify category
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
		
		print(currentCategory + " =>	" + line);
	}
	file.close();
	print("Config file closed");
	return (0);
}
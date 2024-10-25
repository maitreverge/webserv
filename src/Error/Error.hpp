#pragma once
#include <iostream>
#include <vector>
#include <map>

class Error
{
	private:
		std::map<int, std::string> _errorPages;

	public:
		Error();
		~Error();
		static Error& getInstance();
		void				handleError(int errorCode, struct Client &client);
		void				handleError(std::string message) const;
		std::vector<char>	buildErrorRequest(int errorCode);
		std::string			getErrorPagePath(int errorCode) const;
		std::vector<char>	stringToVector(std::string& str);
};
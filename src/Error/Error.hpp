#pragma once
#include <iostream>
#include <vector>
#include <map>

class Error
{
	private:
		std::vector<std::string> _errorPages;

	public:
		Error();
		~Error();
		static Error& getInstance();
		void				handleError(unsigned long errorCode, struct Client &client);
		void				handleError(std::string message) const;
		std::vector<char>	buildErrorRequest(unsigned long errorCode);
		std::string			getErrorPagePath(unsigned long errorCode) const;
		std::vector<char>	stringToVector(std::string& str);
};

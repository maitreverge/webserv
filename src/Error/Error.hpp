#pragma once
#include <iostream>
#include <vector>
#include <map>

class Error
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif
	
		std::vector<std::string> 	_errorPages;
		int							_erorCode;

		std::vector<char>	buildErrorRequest(unsigned long errorCode);
		std::string			getErrorPagePath(unsigned long errorCode) const;
		std::vector<char>	stringToVector(std::string& str);
	public:
		Error();
		~Error();
		static Error& 		getInstance();
		int					getErrorCode() const;
		std::vector<char>	handleError(unsigned long errorCode, struct Client &client);
		void				handleError(std::string message) const;
};

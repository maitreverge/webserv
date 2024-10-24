#pragma once
#include <iostream>
class Error
{
	private:

	public:
		Error();
		~Error();
		static Error& getInstance();
		void	handleError(int errorCode, struct Client &client) const;
		void	handleError(std::string message) const;

};
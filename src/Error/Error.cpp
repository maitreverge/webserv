#include "Error.hpp"
#include "Logger.hpp"

Error::Error() {}
Error::~Error() {}

Error& Error::getInstance()
{
	static Error instance;
	return instance;
}

void	Error::handleError(int errorCode, Client *client) const
{
	if (client == NULL)
		Logger::getInstance().log(ERROR, "Error");
	else
	{
		std::string str;
		std::ostringstream oss;

		oss << errorCode	;
		str = oss.str();
		Logger::getInstance().log(ERROR, str);
	}
}

void	Error::handleError(std::string message) const
{
	Logger::getInstance().log(ERROR, message);
}

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
		Logger::getInstance().log(ERROR, "***********************************just a test***********************************");
	else
	{
		std::string str;
		std::ostringstream oss;

		oss << errorCode	;
		str = oss.str();
		Logger::getInstance().log(ERROR, str);
	}
}

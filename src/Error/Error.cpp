#include "Error.hpp"
#include "Logger.hpp"
#include "master.hpp"

Error::Error() {}
Error::~Error() {}

Error& Error::getInstance()
{
	static Error instance;
	return instance;
}

void	Error::handleError(int errorCode, Client &client) const
{
	std::string str = string("DEINE MUTTER LUTSCHT SCHAENZE IN DER HOELLE!!!");
	Logger::getInstance().log(ERROR, str, client, *this);
	client.statusCode = static_cast<e_errorCodes>(errorCode);
}

void	Error::handleError(std::string message) const
{
	Logger::getInstance().log(ERROR, message);
}

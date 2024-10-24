#include "Error.hpp"
#include "Logger.hpp"

Error::Error() {}
Error::~Error() {}

Error& Error::getInstance()
{
	static Error instance;
	return instance;
}

void	Error::handleError() const
{
	Logger::getInstance().log(ERROR, "***********************************just a test***********************************");
}
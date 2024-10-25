#include "Error.hpp"
#include "Logger.hpp"
#include "master.hpp"
#include "errorCode.hpp"
Error::Error() {
					_errorPages[400] = "errorPages/400.html";
					_errorPages[401] = "errorPages/401.html";
					_errorPages[403] = "errorPages/403.html";
					_errorPages[404] = "errorPages/404.html";
					_errorPages[500] = "errorPages/500.html";
					_errorPages[502] = "errorPages/502.html";
					_errorPages[503] = "errorPages/503.html";
					_errorPages[504] = "errorPages/504.html";
}
Error::~Error() {}

Error& Error::getInstance()
{
	static Error instance;
	return instance;
}

void	Error::handleError(int errCode, Client &client)
{
	errorCode err;

	std::string str = err.getCode(static_cast<const e_errorCodes>(errCode));
	buildErrorRequest(errCode);
	Logger::getInstance().log(ERROR, str, client, *this);
	client.statusCode = static_cast<e_errorCodes>(errCode);
}

void	Error::handleError(std::string message) const
{
	Logger::getInstance().log(ERROR, message);
}

std::string Error::getErrorPagePath(int errorCode) const
{
	std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
	
	if (it != _errorPages.end())
		return it->second;
	else
		return "Default Error Page"; // add a default error page?
}

std::vector<char> Error::stringToVector(std::string& str)
{
	return std::vector<char>(str.begin(), str.end());
}

std::vector<char>	Error::buildErrorRequest(int errorCode)
{
	std::string	errorRequestString;
	std::vector<char>	ErrorRequestVector;

	errorRequestString = "GET " + getErrorPagePath(errorCode) + " HTTP/1.1";
	ErrorRequestVector = stringToVector(errorRequestString);
	return (ErrorRequestVector);
}


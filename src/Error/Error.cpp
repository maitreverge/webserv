#include "Error.hpp"
#include "Logger.hpp"
#include "master.hpp"
#include "errorCode.hpp"

/**========================================================================
 *                           INIT && GETTERS
 *========================================================================**/
Error::Error() {
					_errorPages.resize(505); // segfault without this!!!
					_errorPages[400] = "errorPages/400.html";
					_errorPages[401] = "errorPages/401.html";
					_errorPages[403] = "errorPages/403.html";
					_errorPages[404] = "errorPages/404.html";
					_errorPages[500] = "errorPages/500.html";
					_errorPages[502] = "errorPages/502.html";
					_errorPages[503] = "errorPages/503.html";
					_errorPages[504] = "errorPages/504.html";
					_errorPages[0]	 = "errorPages/default.html";
}

Error::~Error() {}

Error& Error::getInstance()
{
	static Error(instance);
	return instance;
}

int	Error::getErrorCode() const { return (_erorCode); }

/**========================================================================
 *                           HANDLEERROR
 *! CLASS NOT FINISHED
 *? => buildErrorRequest produces right vector, to be sent back to RequestParser
 *? => _errorPages to be initialized during config file parsing
 *========================================================================**/
void	Error::handleError(unsigned long errCode, Client &client)
{
	_erorCode = static_cast<int>(errCode);
	errorCode err;
	std::string str;
	e_errorCodes e_errCode;

	e_errCode = static_cast<e_errorCodes>(errCode);
	str = err.getCode(e_errCode);
	Logger::getInstance().log(ERROR, str, client, *this);
	buildErrorRequest(errCode); //!!! DO SOMETHING WITH THIS !!!
	client.statusCode = e_errCode;
}

void	Error::handleError(std::string message) const
{
	Logger::getInstance().log(ERROR, message);
	// other things to be done?
}


std::string Error::getErrorPagePath(unsigned long errorCode) const
{
	if (!_errorPages[errorCode].empty())
		return (_errorPages[errorCode]);
	else
		return (_errorPages[0]);
}

std::vector<char> Error::stringToVector(std::string& str)
{
	return std::vector<char>(str.begin(), str.end());
}

std::vector<char>	Error::buildErrorRequest(unsigned long errorCode)
{
	std::string	errorRequestString;
	std::vector<char>	ErrorRequestVector;

	errorRequestString = "GET " + getErrorPagePath(errorCode) + " HTTP/1.1";
	ErrorRequestVector = stringToVector(errorRequestString);
	return (ErrorRequestVector);
}


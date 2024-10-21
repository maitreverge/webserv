#include "RequestParser.hpp"
#include "../../includes/master.hpp"
#include "Logger.hpp"
#include "Server.hpp"

/**========================================================================
 *                  CONSTRUCTORS / DESTRUCTOR / INIT
 *========================================================================**/
Headers::							Headers() :
									Connection(""),
									ContentType(""),
									Host(""),
									Accept(),
									ContentLength(0),
									Cookie() {}

RequestParser::RequestParser() : 	_Client(NULL),
									_method(""),
									_URI(""),
									_HTTP_version(""),
									_isValid(true),
									_Headers() {}

RequestParser::~RequestParser() {}

/**========================================================================
 *                           GETTERS
 *========================================================================**/
std::string	RequestParser::getMethod() const 		{return (_method);}

std::string	RequestParser::getURI() const 			{return (_URI);}

std::string	RequestParser::getHTTP_version() const	{return (_HTTP_version);}

Headers	RequestParser::getHeaders() const			{return (_Headers);}

Client*	RequestParser::getClient() const			{return (_Client);}

/**========================================================================
 *                           UTILS
 *========================================================================**/
std::string	RequestParser::charVectorToString(const std::vector<char>& vector)
{
	return (std::string(vector.begin(), vector.end()));
}

void	RequestParser::trim(std::string& str)
{
	str.erase(0, str.find_first_not_of(" \t"));
	str.erase(str.find_last_not_of(" \t") + 1);
}
/**========================================================================
 *                           ACTION
 *========================================================================**/
void	RequestParser::parse(Client& client)
{
	_Client = &client;
	std::istringstream requestStream(charVectorToString(_Client->message));

	handleFirstLine(requestStream);
	handleHeaderLines(requestStream);
	extractHeaders();
	Logger::getInstance().log("Request parsed", *this);
	_Client = NULL;
}

void	RequestParser::handleFirstLine(std::istringstream& requestStream)
{
	std::string firstLine;
	std::string remainingData;

	if (std::getline(requestStream, firstLine))
	{
		std::istringstream iss(firstLine);
		iss >> _method >> _URI >> _HTTP_version;
		if (iss >> 	remainingData || _method.empty() || _URI.empty() || _HTTP_version.empty())
			_isValid = false;
		if (_HTTP_version != "HTTP/1.1")
			_isValid = false;
		if (_method != "GET" && _method != "POST" && _method != "DELETE")
			_isValid = false;
	}
	if (_isValid == 0)
		Logger::getInstance().log("Request first line wrong", *this);
}

void	RequestParser::handleHeaderLines(std::istringstream& requestStream)
{
	std::string headerLine;
	while (std::getline(requestStream, headerLine) && !headerLine.empty())
	{
		size_t colonPos = headerLine.find(':');
		if (colonPos != std::string::npos)
		{
			std::string key = headerLine.substr(0, colonPos);
			std::string value = headerLine.substr(colonPos + 1);
			trim(key); trim(value);
			std::istringstream valueStream(value);
			std::string singleValue;
			while (std::getline(valueStream, singleValue, ','))
			{
				trim(singleValue);
				_tmpHeaders[key].push_back(singleValue);
			}
		}
	}

}

void	RequestParser::extractHeaders()
{
	assignHeader("Host", _Headers.Host);
	assignHeader("Connection", _Headers.Connection);
	assignHeader("Content-Type", _Headers.ContentType);
	assignHeader("Accept", _Headers.Accept);
	assignHeader("Content-Length", _Headers.ContentLength);
	assignHeader("Cookie", _Headers.Cookie);
}

void	RequestParser::assignHeader(const std::string& key, std::string& headerField)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = _tmpHeaders.find(key);

	if (it != _tmpHeaders.end() && !it->second.empty())
		headerField = it->second[0];
}

void	RequestParser::assignHeader(const std::string& key, std::vector<std::string>& headerField)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = _tmpHeaders.find(key);

	if (it != _tmpHeaders.end() && !it->second.empty())
	{
		for (std::vector<std::string>::const_iterator valueIt = it->second.begin();
		     valueIt != it->second.end(); ++valueIt)
		{
			bool exists = false;
			for (std::vector<std::string>::const_iterator it = headerField.begin();
			     it != headerField.end(); it++)
				if (*it == *valueIt)
					exists = true;
			if (!exists)
				headerField.push_back(*valueIt);
		}
	}
}

void	RequestParser::assignHeader(const std::string& key, size_t& headerField)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = _tmpHeaders.find(key);
	if (it != _tmpHeaders.end() && !it->second.empty())
		headerField = static_cast<size_t>(atoi(it->second[0].c_str()));
}

void	RequestParser::assignHeader(const std::string& key, std::map<std::string, std::string>& headerField)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = _tmpHeaders.find(key);
	if (it != _tmpHeaders.end() && !it->second.empty())
		headerField = extractCookies(it->second);
}

std::map<std::string, std::string> RequestParser::extractCookies(std::vector<std::string> vec)
{
	std::map<std::string, std::string> cookiesMap;

	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		size_t equalPos = it->find('=');
		if (equalPos != std::string::npos)
		{
			std::string key = it->substr(0, equalPos);
			std::string value = it->substr(equalPos + 1);
			trim(key);
			trim(value);
			cookiesMap[key] = value;
		}
	}
	return cookiesMap;
}

/**========================================================================
 *                           DISPLAY
 *========================================================================**/
void RequestParser::displayParsingResult() const
{
	displayAttributes();
	displayHeaders(); //! displayHeaders to be taken out of displayAttributes
}

void RequestParser::displayAttributes() const
{
	print("------ ATTRIBUTES ------");
	print("Method: " + _method);
	print("URI: " + _URI);
	print("HTTP_VERSION: " + _HTTP_version);
	printNoEndl("_isValid: "); print(_isValid);
	displayHeaders();
}

void		RequestParser::displayHeaders() const
{
	Logger::getInstance().log("************ HELLO FROM LOGGER!!! ************", *this);
	print("-------- HEADERS -------");
	if (_Headers.Connection.length())
		print("Connection: " + _Headers.Connection);
	if (_Headers.ContentType.length())
		print("ContentType: " + _Headers.ContentType);
	if (_Headers.Host.length())
		print("Host: " + _Headers.Host);
	if (_Headers.Accept.size())
	{
		print("Accept: ");
		for (size_t i = 0; i < _Headers.Accept.size(); i++)
			print("	" + _Headers.Accept[i]);
	}
	std::cout << "ContentLength: " << _Headers.ContentLength << std::endl; 
	if (_Headers.Cookie.size())
	{
		std::map<std::string, std::string>::const_iterator it = _Headers.Cookie.begin();
		print("Cookie: ");
		for (; it != _Headers.Cookie.end(); it++)
			print("	" + it->first + " => " + it->second);
	}
}

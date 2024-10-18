#include "RequestParser.hpp"
#include "../../includes/master.hpp"

/**========================================================================
 *                  CONSTRUCTORS / DESTRUCTOR / INIT
 *========================================================================**/
Headers::Headers() :
	Connection(""),
	ContentType(""),
	Host(""),
	Accept(),
	ContentLength(0),
	Cookie() {}

RequestParser::RequestParser() : 	_method(""),
									_URI(""),
									_HTTP_version(""),
									_isValid(true),
									_Headers() {}

RequestParser::~RequestParser() {}

/**========================================================================
 *                           GETTERS
 *========================================================================**/
std::string							RequestParser::getMethod() const
{
	return (_method);
}

std::string							RequestParser::getURI() const
{
	return (_URI);
}

std::string							RequestParser::getHTTP_version() const
{
	return (_HTTP_version);
}

Headers	RequestParser::getHeaders() const
{
	return (_Headers);
}

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
/**========================================================================
 *                           PARSE
 * ! tmp_http_request_vector to be renamed to http_request_vector
 * ! once connection to kernel done
 *========================================================================**/
void	RequestParser::parse(const std::vector<char> http_request_vector)
{
	// declare char* str
	// const char *http_request_c =
	// 				"GET /index.html HTTP/1.1\r\n"
	// 				"    Host: localhost\r\n"
	// 				"Accept: text/html ,  text/plain           \r\n"
	// 				"Content-Type       :    text/css    \r\n"
	// 				"               Content-Length: 1432\r\n"
	// 				"               Cookie: text=html, kapouet=pouic\r\n"
	// 				"Connection       :    keep-alive       , close   \r\n"
	// 				"\r\n";
	// convert to vector
	// std::vector<char> http_request_vector(http_request_c, http_request_c + strlen(http_request_c));
	std::istringstream requestStream(charVectorToString(http_request_vector));
	handleFirstLine(requestStream);
	handleHeaderLines(requestStream);
	extractHeaders();
	// (void)tmp_http_request_vector;
}

void	RequestParser::handleFirstLine(std::istringstream& requestStream)
{
	std::string firstLine;

	if (std::getline(requestStream, firstLine))
	{
		std::istringstream iss(firstLine);
		iss >> _method >> _URI >> _HTTP_version;
		std::string remainingData;
		if (iss >> 	remainingData || _method.empty() || _URI.empty() || _HTTP_version.empty())
			_isValid = false;
		if (_HTTP_version != "HTTP/1.1")
			_isValid = false;
		if (_method != "GET" && _method != "POST" && _method != "DELETE")
			_isValid = false;
	}

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

void	RequestParser::extractHeaders()
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = _tmpHeaders.begin();
	it = _tmpHeaders.find("Host");
	if (it != _tmpHeaders.end() && !it->second.empty())
		_Headers.Host = it->second[0];
	it = _tmpHeaders.find("Connection");
	if (it != _tmpHeaders.end() && !it->second.empty())
		_Headers.Connection = it->second[0];
	it = _tmpHeaders.find("Content-Type");
	if (it != _tmpHeaders.end() && !it->second.empty())
		_Headers.ContentType = it->second[0];
	it = _tmpHeaders.find("Accept");
	if (it != _tmpHeaders.end() && !it->second.empty())
		_Headers.Accept = it->second;
	it = _tmpHeaders.find("Content-Length");
	if (it != _tmpHeaders.end() && !it->second.empty())
		_Headers.ContentLength = atoi(it->second[0].c_str());
	it = _tmpHeaders.find("Cookie");
	if (it != _tmpHeaders.end() && !it->second.empty())
		_Headers.Cookie = extractCookies(it->second);
}

/**========================================================================
 *                           DISPLAY
 *========================================================================**/
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
	print("-------- HEADERS -------");
	print("Connection: " + _Headers.Connection);
	print("ContentType: " + _Headers.ContentType);
	print("Host: " + _Headers.Host);
	printNoEndl("Accept: ");
	for (size_t i = 0; i < _Headers.Accept.size(); ++i)
	{
		printNoEndl(_Headers.Accept[i]);
		if (i < _Headers.Accept.size() - 1)
			printNoEndl(", ");
	}
	print("");
	std::cout << "ContentLength: " << _Headers.ContentLength << std::endl; 
	std::map<std::string, std::string>::const_iterator it = _Headers.Cookie.begin();
	print("Cookie: ");
	for (; it != _Headers.Cookie.end(); it++)
	{
		print("	" + it->first + " => " + it->second);
	}
}

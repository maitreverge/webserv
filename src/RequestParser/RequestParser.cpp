#include "RequestParser.hpp"
#include "../../includes/master.hpp"

RequestParser::RequestParser() : 
_method(""),
_URI(""),
_HTTP_version(""),
isValid(true)
{
	// print("RequestParser class created\n");
}

RequestParser::~RequestParser()
{
	// print("RequestParser class destroyed\n");
}

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

std::map<std::string, std::vector<std::string> >	RequestParser::getHeaders() const
{
	return (_headers);
}

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
 *                           PARSE
 * ! tmp_http_request_vector to be renamed to http_request_vector
 * ! once connection to kernel done
 *========================================================================**/
void	RequestParser::parse(const std::vector<char>& tmp_http_request_vector)
{
	// declare char* str
	const char *http_request_c =
					"GET /index.html HTTP/1.1\r\n"
					"    Host: localhost\r\n"
					"Accept: text/html ,  text/plain           \r\n"
					"Content-Type       :    text/html    \r\n"
					"               Content-Length: 1432\r\n"
					"               Cookie: key=value, kapouet=pouic\r\n"
					"Connection       :    keep-alive       , close   \r\n"
					"\r\n";
	// convert to vector
	std::vector<char> http_request_vector(http_request_c, http_request_c + strlen(http_request_c));
	std::istringstream requestStream(charVectorToString(http_request_vector));
	handleFirstLine(requestStream);
	handleHeaderLines(requestStream);
	extractHeaders();

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
			isValid = false;
		if (_HTTP_version != "HTTP/1.1")
			isValid = false;
		if (_method != "GET" && _method != "POST" && _method != "DELETE")
			isValid = false;
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
				_headers[key].push_back(singleValue);
			}
		}
	}

}

void	RequestParser::extractHeaders()
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = _headers.begin();
	for (it;it != _headers.end(); ++it)
	{
		std::string output = it->first + ": ";
		for (size_t i = 0; i < it->second.size(); i++)
		{
			output += it->second[i];
			if (i < it->second.size() - 1)
				output += ", ";
		}
		print(output);
	}

	it = _headers.find("Host");
	if (it != _headers.end() && !it->second.empty())
		_Headers.Host = it->second[0];
	it = _headers.find("Connection");
	if (it != _headers.end() && !it->second.empty())
		_Headers.Connection = it->second[0];
	it = _headers.find("Content-Type");
	if (it != _headers.end() && !it->second.empty())
		_Headers.ContentType = it->second[0];
	it = _headers.find("Accept");
	if (it != _headers.end() && !it->second.empty())
		_Headers.Accept = it->second;
	it = _headers.find("Content-Length");
	if (it != _headers.end() && !it->second.empty())
		_Headers.ContentLength = atoi(it->second[0].c_str());
	

}

void RequestParser::displayAttributes() const
{
	print("Method: " + _method);
	print("URI: " + _URI);
	print("HTTP_VERSION: " + _HTTP_version);
	printNoEndl("isValid: "); print(isValid);
	std::map<std::string, std::vector<std::string> >::const_iterator it = _headers.begin();
	for (it;it != _headers.end(); ++it)
	{
		std::string output = it->first + ": ";
		for (size_t i = 0; i < it->second.size(); i++)
		{
			output += it->second[i];
			if (i < it->second.size() - 1)
				output += ", ";
		}
		print(output);
	}
}

void		RequestParser::displayHeaders() const
{
	print("HEADERS:");
	print("Connection: " + _Headers.Connection);
	print("ContentType: " + _Headers.ContentType);
	print("Host: " + _Headers.Host);
	printNoEndl("Accept: ");
	std::vector<std::string>::const_iterator it = _Headers.Accept.begin();
	for (size_t i = 0; i < _Headers.Accept.size(); ++i)
	{
		printNoEndl(_Headers.Accept[i]);
		if (i < _Headers.Accept.size() - 1)
			printNoEndl(", ");
	}
	print("");
	std::cout << "ContentLength: " << _Headers.ContentLength << std::endl; 
	print("Cookie: ");
}

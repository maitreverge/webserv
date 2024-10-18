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

std::map<std::string, std::string>	RequestParser::getHeaders() const
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

void	RequestParser::handleFirstLine(std::vector<char> http_request_vector)
{
	std::istringstream requestStream(charVectorToString(http_request_vector));
	std::string firstLine;
	if (std::getline(requestStream, firstLine))
	{
		std::istringstream iss(firstLine);
		iss >> _method >> _URI >> _HTTP_version;
		std::string remainingData;
		if (iss >> 	remainingData || _method.empty() || _URI.empty() || _HTTP_version.empty())
		{
			isValid = false;
		}
		if (_HTTP_version != "HTTP/1.1")
			isValid = false;
		if (_method != "GET" && _method != "POST" && _method != "DELETE")
			isValid = false;
	}

}

void	RequestParser::handleHeaders(std::vector<char> http_request_vector)
{
	std::istringstream requestStream(charVectorToString(http_request_vector));
	std::string headerLine;
	while (std::getline(requestStream, headerLine) && !headerLine.empty())
	{
		size_t colonPos = headerLine.find(':');
		if (colonPos != std::string::npos) {
			std::string key = headerLine.substr(0, colonPos);
			std::string value = headerLine.substr(colonPos + 1);
			trim(key);
			trim(value);
			_headers[key] = value;
		}
	}

}

void	RequestParser::parse(const std::vector<char>& data)
{
	// declare char* str
	const char *http_request_c = "GET /index.html HTTP/1.1\r\n"
							"    Host: localhost\r\n"
							"User-Agent       :        Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.93 Safari/537.36\r\n"
							"Accept: text/html             \r\n"
							"               Connection: keep-alive\r\n"
							"\r\n";
	
	// convert to vector
	std::vector<char> http_request_vector(http_request_c, http_request_c + strlen(http_request_c));

	// declar vars to get first line
	handleFirstLine(http_request_vector);
	handleHeaders(http_request_vector);
}


void	RequestParser::displayAttributes() const
{
	printNoEndl("Method:");
	print(_method);
	printNoEndl("URI:");
	print(_URI);
	printNoEndl("HTTP_VERSION:");
	print(_HTTP_version);
	printNoEndl("isValid:");
	print(isValid);
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		print(it->first + ":" + it->second);
	}
}
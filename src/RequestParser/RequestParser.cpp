#include "RequestParser.hpp"
#include "../../includes/master.hpp"

RequestParser::RequestParser() : 
_method(""),
_URI(""),
_HTTP_version(""),
isValid(0)
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

void	RequestParser::parse(const std::vector<char>& data)
{
	// declare char* str
	const char *http_request_c = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	// convert to vector
	std::vector<char> http_request_vector(http_request_c, http_request_c + strlen(http_request_c));

	// declar vars to get first line
	std::istringstream requestStream(charVectorToString(http_request_vector));
	std::string firstLine;

	if (std::getline(requestStream, firstLine))
	{
		RequestLine parsedRequest = parseRequestLine(charVectorToString(http_request_vector));
		_method = parsedRequest.method;
		_URI = parsedRequest.uri;
		_HTTP_version = parsedRequest.http_version;
	}
}



RequestLine RequestParser::parseRequestLine(const std::string& requestLine)
{
	RequestLine request;

	std::istringstream iss(requestLine);
	iss >> request.method >> request.uri >> request.http_version;

	return request;
}

void	RequestParser::displayAttributes() const
{
	print("Attributes:\n");	
	printNoEndl("Method: ");
	print(_method);
	printNoEndl("URI: ");
	print(_URI);
	printNoEndl("HTTP_VERSION: ");
	print(_HTTP_version);
	printNoEndl("isValid: ");
	print(isValid);
	print("Headers:\n");
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		print(it->first + ": " + it->second);
	}
}
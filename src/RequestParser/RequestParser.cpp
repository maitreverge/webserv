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

void	RequestParser::parse(const std::vector<char>& data)
{
	// declare char* str
	const char *http_request_c = "GET /index.html HTTP/1.1\r\n"
							"Host: localhost\r\n"
							"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.93 Safari/537.36\r\n"
							"Accept: text/html\r\n"
							"Connection: keep-alive\r\n"
							"\r\n";
	
	// convert to vector
	std::vector<char> http_request_vector(http_request_c, http_request_c + strlen(http_request_c));

	// display vector
	// print("Display Vector:");
	// for (std::vector<char>::const_iterator it = http_request_vector.begin(); it != http_request_vector.end(); it++)
	// {
	// 	std::cout << *it;
	// }


	// declar vars to get first line
	std::istringstream requestStream(charVectorToString(http_request_vector));
	std::string firstLine;

	if (std::getline(requestStream, firstLine))
	{
		RequestLine parsedRequest = parseRequestLine(firstLine);
		_method = parsedRequest.method;
		_URI = parsedRequest.uri;
		_HTTP_version = parsedRequest.http_version;
	}

	std::string headerLine;
	while (std::getline(requestStream, headerLine) && !headerLine.empty()) {
		size_t colonPos = headerLine.find(':');
		if (colonPos != std::string::npos) {
			std::string key = headerLine.substr(0, colonPos);
			std::string value = headerLine.substr(colonPos + 1); // Récupérer tout ce qui suit le ':'
			// Éliminer les espaces au début et à la fin du value
			
			key.erase(0, key.find_first_not_of(" \t")); // Supprimer les espaces au début
			key.erase(key.find_last_not_of(" \t") + 1); // Supprimer les espaces à la fin
			value.erase(0, value.find_first_not_of(" \t")); // Supprimer les espaces au début
			value.erase(value.find_last_not_of(" \t") + 1); // Supprimer les espaces à la fin
			_headers[key] = value; // Ajouter le header au map
		}
	}
}



RequestLine RequestParser::parseRequestLine(const std::string& requestLine)
{
	RequestLine request;

	std::istringstream iss(requestLine);
	iss >> request.method >> request.uri >> request.http_version;
	std::string remainingData;
	if (iss >> remainingData || request.method.empty() || request.uri.empty() || request.http_version.empty())
	{
		isValid = false;
	}
	std::cout << request.http_version << std::endl;
	std::cout << request.method << std::endl;
	std::cout << request.uri << std::endl;
	return request;
}

void	RequestParser::displayAttributes() const
{
	// printNoEndl("Method:");
	// print(_method);
	// printNoEndl("URI:");
	// print(_URI);
	// printNoEndl("HTTP_VERSION:");
	// print(_HTTP_version);
	printNoEndl("isValid:");
	print(isValid);
	// for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
	// {
	// 	print(it->first + ":" + it->second);
	// }
}
#include "RequestParser.hpp"
#include "../../includes/master.hpp"

RequestParser::RequestParser()
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
	const char *http_request_c = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	
	std::vector<char> http_request_vector(http_request_c, http_request_c + strlen(http_request_c));

	print(charVectorToString(http_request_vector));
}

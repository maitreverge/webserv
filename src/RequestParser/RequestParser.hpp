#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>

struct RequestLine {
    std::string method;
    std::string uri;
    std::string http_version;
};

class RequestParser
{
	private:
		std::string							_method;
		std::string							_URI;
		std::string							_HTTP_version;
		bool								isValid;
		std::map<std::string, std::string>	_headers;

		RequestParser(const RequestParser& other);
		RequestParser& operator=(const RequestParser& rhs);
	public:
		//coplien
		RequestParser();
		~RequestParser();
		
		// getters
		std::string	getMethod() const;
		std::string	getURI() const;
		std::string	getHTTP_version() const;
		std::map<std::string, std::string>	getHeaders() const;
		
		void		handleHeaders(std::istringstream& requestStream);
		void		handleFirstLine(std::istringstream& requestStream);
		std::string	charVectorToString(const std::vector<char>& vector);
		void		parse(const std::vector<char>& data);
		RequestLine parseRequestLine(const std::string& requestLine);
		void		trim(std::string& str);
		void		displayAttributes() const;
};
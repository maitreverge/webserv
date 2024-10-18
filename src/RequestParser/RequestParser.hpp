#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>

struct Headers {
	std::string							Connection;
	std::string							ContentType;
	std::string							Host;
	std::vector<std::string>			Accept;
	u_int32_t							ContentLength;
	std::map<std::string, std::string>	Cookie;
};

class RequestParser
{
	private:
		std::string											_method;
		std::string											_URI;
		std::string											_HTTP_version;
		bool												isValid;
		std::map<std::string, std::vector<std::string> >	_headers;
		Headers												_Headers;

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
		std::map<std::string, std::vector<std::string> >	getHeaders() const;
		std::map<std::string, std::string>	extractCookies(std::vector<std::string> vec);
		void		handleHeaderLines(std::istringstream& requestStream);
		void		handleFirstLine(std::istringstream& requestStream);
		std::string	charVectorToString(const std::vector<char>& vector);
		void		parse(const std::vector<char>& data);
		void		trim(std::string& str);
		void		displayAttributes() const;
		void		displayHeaders() const;
		void		extractHeaders();

};
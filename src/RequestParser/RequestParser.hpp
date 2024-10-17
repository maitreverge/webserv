#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>

class RequestParser
{
	private:
		std::string							_method;
		std::string							_URI;
		std::string							_HTTP_version;
		std::map<std::string, std::string>	_headers;
		bool								isValid;

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
		
		std::string	charVectorToString(const std::vector<char>& vector);
		void		parse(const std::vector<char>& data); // will set the attributes
};
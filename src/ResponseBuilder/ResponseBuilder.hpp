#pragma once

#include "utils_templates.hpp"
#include "Logger.hpp"
#include "../errorCode/errorCode.hpp"
#include "timeStamp.hpp"

#define HTTP_PROTOCOL "HTTP/1.1"
#define SPACE " "
#define HTTP_REPONSE_SEPARATOR "\r\n\r\n"

struct Client;
struct Config;

struct ResponseHeaders
{
	vector<char> masterHeader;
	
	string statusLine;
	string timeStamp;
	string contentType;
	string transfertEncoding; // ! OPTION ONE
	// !           OR
	string contentLenght; // ! OPTION TWO

	// Utils
	u_int64_t bodyLenght;
	
	ResponseHeaders()
	{
		masterHeader.clear();
		bodyLenght = 0; // ? doublon par rapport a la struct client 

	}
};

class ResponseBuilder
{

private:

	// ------------- Coplian Useless Methods
	ResponseBuilder( const ResponseBuilder& copy );
	ResponseBuilder& operator=( const ResponseBuilder& right_operator );

	// ------------- Priv Variables
	bool _isHeaderDone; // master bool
	
	map<string, string> _mimeTypes;
	bool _isDirectory;
	bool _isCGI;
	struct stat _fileInfo;

	string _realURI;
	e_errorCodes _errorType;

	Client* _client;

	ResponseHeaders Headers;

	// ------------- Priv Methods
	void	resolveURI( Client&, Config& );
	void	sanatizeURI( string & );
	void	initialChecks( Client&, Config& );
	void	launchCGI( void );
	void	buildHeaders( void );
	void	setContentLenght(); // not a regular setter





public:

	ResponseBuilder( void );
	~ResponseBuilder();

	vector<char>	getHeader( Client &, Config& );
	void			getBody( Client &, Config& );

};
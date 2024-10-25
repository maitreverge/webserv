#pragma once

#include "utils_templates.hpp"
#include "errorCode.hpp"
#include "timeStamp.hpp"
#include "Config.hpp"

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

	// Enum
	typedef enum
	{
		GET,
		POST,
		DELETE
	} e_method;

	// ------------- Coplian Useless Methods

	// ------------- Priv Variables
	
	map<string, string> _mimeTypes;
	bool _isDirectory;
	bool _isFile;
	bool _isCGI;
	struct stat _fileInfo;

	string _fileName;
	string _fileExtension;
	string _realURI;
	e_errorCodes _errorType;
	e_method _method;


	Client* _client;
	Config* _config;

	ResponseHeaders Headers;


	// ------------- Priv Methods
	void	resolveURI( void );
	void	sanatizeURI( string & );
	void	validateURI( void );
	void	launchCGI( void );
	void	buildHeaders( void );
	void	setContentLenght(); // not a regular setter
	void	extractMethod( void );
	void	setError( e_errorCodes );
	string	extractType( const string& extension ) const;


public:

	ResponseBuilder( void );
	~ResponseBuilder();
	
	// std::ifstream _bodyStream;
	bool _headerSent; // master bool

	void	getHeader( Client &, Config& );
	bool	getBody( void );
};
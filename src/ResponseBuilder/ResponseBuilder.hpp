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

	// Enum
	typedef enum
	{
		GET,
		POST,
		DELETE
	} e_method;

	// ------------- Coplian Useless Methods
	ResponseBuilder( const ResponseBuilder& copy );
	ResponseBuilder& operator=( const ResponseBuilder& right_operator );

	// ------------- Priv Variables
	bool _headerSent; // master bool
	
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

	vector<char>	getHeader( Client &, Config& );
	void			getBody( Client &, Config& );

};
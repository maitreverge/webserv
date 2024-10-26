#pragma once

#include "utils_templates.hpp"
#include "errorCode.hpp"
#include "timeStamp.hpp"
#include "Config.hpp"

#define HTTP_PROTOCOL "HTTP/1.1"
#define SPACE " "
#define HTTP_HEADER_SEPARATOR "\r\n"

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
	uint64_t bodyLenght;

	// ! ADD COCKIES HERE
	
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
	void	buildHeaders( void );
	void	setContentLenght( void ); // not a regular setter
	void	extractMethod( void );
	void	setError( e_errorCodes );
	string	extractType( const string& extension ) const;
	void	initMimes( void );

	// generateListingHTML.cpp
	map<string, timespec> _lastDir_M_Time;
	map<string, timespec> _lastDir_C_Time;
	void	generateListingHTML( void );
	bool	foundDefaultPath( void );
	bool	isDirectoryUnchanged( void );



	// CGI.cpp
	void	launchCGI( void );



public:

	ResponseBuilder( void );
	~ResponseBuilder();
	ResponseBuilder(const ResponseBuilder &);
	ResponseBuilder & operator=(const ResponseBuilder & rhs)
	{
		this->_client = rhs._client;
		this->_config = rhs._config;
		this->_mimeTypes = rhs._mimeTypes;
		this->_headerSent = rhs._headerSent;

		this->Headers = rhs.Headers;
		this->_method = rhs._method;
		this->_errorType = rhs._errorType;
		this->_realURI = rhs._realURI;
		this->_fileExtension = rhs._fileExtension;
		this->_fileInfo = rhs._fileInfo;			
		this->_isDirectory = rhs._isDirectory;	
		this->_isFile = rhs._isFile;
		this->_isCGI = rhs._isCGI;
		this->_fileName = rhs._fileName;	
	
		return *this;
	};

	bool _headerSent;
	std::ifstream _ifs;

	void	getHeader( Client &, Config& );
	ssize_t	getBody( Client &inputClient );

	void	printAllHeaders( void );

};
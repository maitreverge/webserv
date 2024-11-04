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
class MockConfig;

struct ResponseHeaders
{
	vector<char> masterHeader;
	
	string statusLine;
	string timeStamp;
	string contentType;
	string transfertEncoding; // ! OPTION ONE
	// !           OR
	string contentLenght; // ! OPTION TWO
	
	// TODO : All Headers
	string location; // Only if redirection

	// Utils
	uint64_t bodyLenght;

	// ! ADD COCKIES HERE
	
	ResponseHeaders()
	{
		masterHeader.clear();
		bodyLenght = 0;
	}
};

struct RouteConfig
{
	vector< string > allowedMethods;
	vector< string > redirection;
	vector< string > rootSearch;
	vector< string > path;
	vector< string > cgiExtensions;
	vector< string > listingDirectories;
	vector< string > uploadDirectory;
	vector< string > uploadDirectory;
	vector< string > routeMapping;
};

class ResponseBuilder
{

	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif

	// Enum
	typedef enum
	{
		GET,
		POST,
		DELETE
	} e_method;

	// ------------- Priv Variables
	
	map<string, string> _mimeTypes;

	string _realURI;
	e_method _method;

	// Nature File
	bool _isDirectory;
	bool _isFile;

	bool _errorNotFound;

	// CGI Stuff
	bool _isCGI;
	e_errorCodes _errorType;
	string _pathInfo;

	// Struct for File Info
	struct stat _fileInfo; // ! PAS DANS LES CONSTRUCTEURS

	// File Characteristics
	bool _isROK;
	bool _isWOK;
	bool _isXOK;
	string _fileName;
	string _fileExtension;

	Client* _client;
	Config* _config;

	ResponseHeaders Headers;

	// bool test;

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
	void	checkNatureAndAuthoURI( void );
	void	checkAutho( void );
	void	checkNature( void );
	bool 	redirectURI( void );
	void 	rootMapping( void );
	bool	isErrorRedirect( void );
	void	extractFileNature( string &target);
	void	checkMethod( void );
	void	extractRouteConfig( void );



	// generateListingHTML.cpp
	/*
		map<string, timespec> _lastDir_M_Time;
		map<string, timespec> _lastDir_C_Time;
		bool	isDirectoryUnchanged( void );
	*/
	void	generateListingHTML( void );
	bool	foundDefaultPath( void );
	void	listingHTMLBuilder( void);


	// CGI.cpp
	void	checkCGI( void );
	void	launchCGI( void );

	// POST

	// DELETE
	void	deleteEngine( void );




public:

	std::ifstream 	_ifs; // ! PAS DANS LES CONSTRUCTEURS
	std::streampos	_ifsStreamHead; // ! ABSOLUMENT METTRE DANS LES CONSTRUCTEURS
    std::ofstream	_ofs;
	std::streampos	_ofsStreamHead; // ! ABSOLUMENT METTRE DANS LES CONSTRUCTEURS

	ResponseBuilder( void );
	~ResponseBuilder( void );
	ResponseBuilder( const ResponseBuilder & );
	ResponseBuilder & operator=( const ResponseBuilder & rhs);

	// ✅ GET ONLY
	void	getHeader( Client &, Config& );
	ssize_t	getBody( Client &inputClient );

	// ✅ POST ONLY
	void	getHeaderPost( Client &inputClient, Config &inputConfig );
	void	setBodyPost( std::vector<char> bodyParts );


	// For testing
	void	setMethod( const e_method& method );

	void	printAllHeaders( void )const;
};
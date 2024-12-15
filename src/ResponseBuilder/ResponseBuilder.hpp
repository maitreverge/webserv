#pragma once

#include "utils_templates.hpp"
#include "errorCode.hpp"
#include "timeStamp.hpp"
#include "Config.hpp"
#include "Cgi.hpp"

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
	string contentLenght;
	string location;
	string cookie;

	// Utils
	uint64_t bodyLenght;

	ResponseHeaders()
	{
		masterHeader.clear();
		bodyLenght = 0;
	}
};

struct MyConfig
{
	// ! main key
	string				uri;

	vector< string >	allowedMethods;
	string				redirection;
	string				root;
	bool				listingDirectory;
	string				index;
	vector< string >	cgiAllowed;
	bool				uploadAllowed;
	string				uploadDirectory;

	// ========== my stuff ==========
	string				indexRedirection;

	MyConfig()
	{
		allowedMethods.clear();
		redirection.clear();
		uri.clear();
		root.clear();
		listingDirectory = false;
		index.clear();
		cgiAllowed.clear();
		uploadAllowed = true;
		uploadDirectory.clear();

	// ========== my stuff ==========
		indexRedirection.clear();
	}
};

class ResponseBuilder
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif

	// ===================== PRIVATE DATA-STRUCTURE ==================
	typedef enum
	{
		TOKEN_DELIM,
		TOKEN_END,
		LINE_SEPARATOR,
		BINARY_DATA,
		CONTENT_DISPOSITION,
		OTHER
	} e_lineNature;

	// ===================== PRIVATE VARIABLES ==================
	
	Client* _client;
	Config* _config;
	
	map<string, string>	_mimeTypes;
	MyConfig			_myconfig;
	ResponseHeaders		Headers;
	e_errorCodes		_errorType;

	struct stat _fileInfo; // ! SURTOUT PAS DANS LES CONSTRUCTEURS

	string _realURI;
	string _originalURI;

	// Nature File
	bool _isDirectory;
	bool _isFile;

	// File Characteristics
	bool _isROK;
	bool _isWOK;
	bool _isXOK;

	// errorNotFoundGenerator
	bool _deleteURI; // used for auto generated errors or listing.html
	string _backupNameFile;

	// Multipart-Form Data Utils
	bool _isServerName;
	bool _isMultipart;
	bool _writeReady;
	bool _parsedBoundaryToken;
	bool _serverNameChecked;

	string _tokenDelim;
	string _tokenEnd;
	string _postFileName;
	string _setBodyExtension;

	string _fileStreamName;


	string _serverName;
	string _uploadTargetDirectory;

	map< string, string >_servernameType;

	vector< string > _forbiddenFiles;

	// ===================== PRIVATE METHODS ==================

	// ---------- buildHeaders.cpp
	void	buildHeaders( void );

	// ---------- CGI.cpp
	void	checkCGI( void );

	// ---------- coockiesAndSession.cpp // ! DAN FUNCTIONS
	void		buildSetCookieHeader( void );
	void		checkSessionIdCookie( Client &inputClient );
	std::string	generateUniqueToken( const std::string& clientIP );

	// ---------- coplianForm.cpp
	void	initMimes( void );
	void	initServerNames( void );
	void	initForbiddenFiles( void );

	// ---------- deleteEngine.cpp
	void	generateDeleteHTML(void);
	void	makeDeleteHTML(std::stringstream &result);
	void	deleteEngine( void );

	// ---------- errorNotFoundGenerator.cpp
	void	errorNotFoundGenerator( void );

	// ---------- extractRouteConfig
	void 	extractRouteConfig(void);
	void 	extraStartingChecks( void );
	void 	resetMyVariables( void );
	void	clearingRoutes( vector< string >&, vector< string >& );
	void	buildRouteConfig( string );
	void	printMyConfig( void );
	void	extractRedirectionIndex( vector< string >& );

	// ---------- generateListingHTML.cpp
	bool	foundDefaultPath( void );
	bool	isFileIgnored( string & );
	void 	listingHTMLBuilder( void );
	void 	makeHeaderListing( std::stringstream & );
	void	generateListingHTML( void );

	// ---------- getBody.cpp //! PUBLIC METHOD

	// ---------- removeDotSegments.cpp
	void removeDotSegments( void );

	// ---------- ResponseBuilder.cpp
	void	resolveURI( void );
	void 	redirectURI( void );
	void 	rootMapping( void );
	void	checkMethod( void );
	bool	isDirectory(string &);
	void	slashManip( string&, bool makeRedirection = false );
	void	printAllHeaders( void )const;

	// ---------- serverName.cpp
	string	parseServerName( string & );
	void 	applyServerName( void );
	void 	serverNameChecking(void);
	void 	extractServerName( void );

	// ---------- setBody.cpp
	void					initCurrentFiles( vector< string> & );
	bool					isLineDelim(vector<char> &, vector<char> &);
	void					determineSeparator(std::string &, size_t &, vector<char>& );
	e_lineNature			processCurrentLine( vector< char >&  );
	void					initBoundaryTokens( void );
	void					extractFileBodyName( vector< char >&, vector< string >& );
	void					setRegularPost( Client & );
	void					setMultiPartPost( Client & );
	vector<char>::iterator	searchSeparator( vector<char>& , string &, size_t & );

	// ---------- utilsResponseBuilder.cpp
	string	extractType( const string& ) const;
	void	extractMethod( void );
	void	setContentLenght( void ); // ! not a regular setter
	void	checkAutho( void );
	void	extractFileNature( string &);
	void	checkNature( void );
	bool	isErrorRedirect( void );
	void	pathSlashs(string &);
	string	generateFileName( void );
	string	generateRandomString( size_t, bool underscoreNeeded = false );
	void	setError( e_errorCodes, bool skip = false );
	bool	isForbiddenFiles( void );


public:

	// ========================== PUBLIC DATA-STRUCTURE ========================

	typedef enum
	{
		GET,
		POST,
		DELETE
	} e_method;

	e_method _method;

	// ========================== PUBLIC NESTED CLASSES ========================

	class CodeErrorRaised : public exception
	{
		public:
			virtual const char* what( void ) const throw()
			{
				return ("CODE ERROR RAISED");
			}
	};

	// ============================== PUBLIC METHODS ===========================
	
	// ---- Streams
	std::ifstream 	_ifs; // ! PAS DANS LES CONSTRUCTEURS
	std::streampos	_ifsStreamHead; // ! ABSOLUMENT METTRE DANS LES CONSTRUCTEURS
    std::ofstream	_ofs;
	
	// ---- CGI related infos
	Cgi				_cgi;
	string 			_pathInfo;
	string 			_folderCGI;
	string 			_fileName;
	string 			_fileExtension;
	
	// ============================= PUBLIC VARIABLES ==========================

	// ---- Coplian Form

	bool 			_isCGI; //! need public for seb

	ResponseBuilder(void);
	~ResponseBuilder( void );
	ResponseBuilder( const ResponseBuilder & );
	ResponseBuilder & operator=( const ResponseBuilder & rhs);

	// ---- Main Functions
	void	getHeader( Client &, Config&, e_errorCodes codeInput = CODE_200_OK );
	bool	getBody( Client & );
	void	setBody( Client &, bool );
	
	// For testing with google testing framework
	void	setMethod( const e_method& );
	e_method getMethod( void );
};

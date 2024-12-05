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
	
	MyConfig _myconfig;
	ResponseHeaders Headers;

	struct stat _fileInfo; // ! PAS DANS LES CONSTRUCTEURS

	map<string, string> _mimeTypes;
	
	string _realURI;
	string _originalURI;


	// Nature File
	bool _isDirectory;
	bool _isFile;

	// CGI Stuff
	bool _isCGI;

	// File Characteristics
	bool _isROK;
	bool _isWOK;
	bool _isXOK;

	// errorNotFoundGenerator
	bool _deleteURI; // used for auto generated errors or listing.html
	string _backupNameFile;

	// CGI Stuff
	e_errorCodes _errorType;

	bool _isMultipart;
	string _setBodyExtension;

	string _tokenDelim;
	string _tokenEnd;
	string _postFileName;

	string _fileStreamName;

	bool _writeReady;
	bool _parsedBoundaryToken;
	bool _serverNameChecked;

	bool _isServerName;
	string _serverName;
	string _uploadTargetDirectory;

	map< string, string >_servernameType;

	// ===================== PRIVATE METHODS ==================

	// ---------- buildHeaders.cpp
	void	buildHeaders( void );

	// ---------- CGI.cpp
	void	checkCGI( void );

	// ---------- coockiesAndSession.cpp // ! DAN FUNCTIONS
	void		buildSetCookieHeader();
	void		checkSessionIdCookie(Client &inputClient);
	std::string	generateUniqueToken(const std::string& clientIP);

	// ---------- coplianForm.cpp
	void	initMimes( void );
	void	initServerNames( void );

	// ---------- deleteEngine.cpp
	void	generateDeleteHTML( void );
	void	deleteEngine( void );

	// ---------- errorNotFoundGenerator.cpp
	void	errorNotFoundGenerator( void );

	// ---------- extractRouteConfig
	void 	extractRouteConfig(void);
	void 	extraStartingChecks();
	void 	resetMyVariables();
	void	clearingRoutes( vector< string >&, vector< string >& );
	void	buildRouteConfig( string );
	void	printMyConfig( void );
	void	extractRedirectionIndex( vector< string >&, vector< string >& );

	// ---------- generateListingHTML.cpp
	bool	foundDefaultPath( void );
	bool	isFileIgnored( string & );
	void 	listingHTMLBuilder(void);
	void 	makeHeaderListing(std::stringstream &);
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

	// ---------- serverName.cpp
	string	parseServerName( string & );
	void 	applyServerName( void );
	void 	serverNameChecking(void);
	void 	extractServerName();

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
	string	generateRandomString(size_t, bool underscoreNeeded = false );

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
	// ============================= PUBLIC VARIABLES ==========================


	std::ifstream 	_ifs; // ! PAS DANS LES CONSTRUCTEURS
	std::streampos	_ifsStreamHead; // ! ABSOLUMENT METTRE DANS LES CONSTRUCTEURS
    std::ofstream	_ofs; //! need public for seb
	string 			_pathInfo; //! need public for seb
	string 			_folderCGI; //! need public for seb
	string 			_fileName; //! need public for seb
	string 			_fileExtension; //! need public for seb

	ResponseBuilder( void );
	~ResponseBuilder( void );
	ResponseBuilder( const ResponseBuilder & );
	ResponseBuilder & operator=( const ResponseBuilder & rhs);

	void	getHeader( Client &, Config&, e_errorCodes codeInput = CODE_200_OK );
	bool	getBody( Client & );
	
	Cgi		_cgi;//! provisoire sinon private

	void	setBody( Client &, bool );


	void	printAllHeaders( void )const;



	// Public method for CGI error timeout
	void	setError( e_errorCodes, bool skip = false );
	e_method getMethod( void );
	// For testing
	void	setMethod( const e_method& );
};

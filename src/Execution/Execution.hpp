#pragma once

#include "../../includes/master.hpp"
#include "utils_templates.hpp"
#include "Logger.hpp"
struct Client;
struct Config;

struct CGI
{
	// timeout max for CGI execution
	string cgiType; // Either ".py" or ".php"

};

class Execution
{

private:

	// ----------- VARIABLES -------------
	bool _isCGI;
	bool _isDirectory;
	bool _isCheckingDone;

	e_errorCodes _errorType;

	string _realURI;

	string _curPath;

	struct stat _fileInfo; // Struct used for `stat` function (equivalent to cpp)

	Client* _client;

	CGI	_cgi;

	// ----------- PRIV METHODS -------------
	void	sanatizeURI( string& );
	void	resolveURI( Client&, Config& );
	void	initialChecks( Client&, Config& );
	void	launchCGI( void ); // TODO
	

protected:

	// Some protected members

public:

	Execution( void );
	Execution( const Execution& copy );
	Execution& operator=( const Execution& right_operator );
	~Execution();

	void callGet( Client&, Config& ); // TODO
	void callPost( Client& ); // TODO
	void callDelete( Client& ); // TODO

};
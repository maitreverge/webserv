#pragma once

#include "../../includes/master.hpp"
#include "utils_templates.hpp"
#include "Logger.hpp"
struct Client;

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

	Client* _client;

	CGI	_cgi;

	// ----------- PRIV METHODS -------------
	void launchCGI( void ); // TODO
	

protected:

	// Some protected members

public:

	Execution( void );
	Execution( const Execution& copy );
	Execution& operator=( const Execution& right_operator );
	~Execution();

	void callGet( Client& ); // TODO
	void callPost( Client& ); // TODO
	void callDelete( Client& ); // TODO

};
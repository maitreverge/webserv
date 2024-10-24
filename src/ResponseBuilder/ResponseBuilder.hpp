#pragma once

#include "utils_templates.hpp"

struct Client;
struct Config;

struct ResponseHeaders
{
	ResponseHeaders()
	{

	}
};

class ResponseBuilder
{

private:

	// ------------- Coplian Useless Methods
	ResponseBuilder( const ResponseBuilder& copy );
	ResponseBuilder& operator=( const ResponseBuilder& right_operator );

	// ------------- Priv Methods


public:

	ResponseBuilder( void );
	~ResponseBuilder();

	vector<char>	getHeader( Client &, Config& );
	void			getBody( Client &, Config& );

};
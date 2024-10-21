#pragma once

#include "utils_templates.hpp"
#include "libraries.hpp"
#include "errorCode.hpp"
#include "timeStamp.hpp"

#define HTTP_REPONSE_SEPARATOR "\r\n"
#define HTTP_PROTOCOL "HTTP/1.1"
#define SPACE " "

class buildResponse
{

private:

	// *		---------------- COPLIAN METHODS ----------------
	buildResponse( const buildResponse& copy );
	buildResponse& operator=( const buildResponse& right_operator );

	// *		---------------- PRIV METHODS ----------------
	string buildContentType( const string& )const;
	string extractType( const string& )const;

	// *		---------------- ATTRIBUTES ----------------
	map<string, string> _mimeTypes;

	// Final Answer
	vector<char> _httpResponse;
	
	// Headers
	string _statusLine;
	string _timeStamp;
	string _contentType;
	string _transfertEncoding; // ! OPTION ONE
	// !           OR
	string _contentLenght; // ! OPTION TWO

	// Main Blocks
	string _masterHeader;
	string _body;

	// Utils
	size_t _bodyLenght;

	// TODO : Create variables for coockies

public:

	buildResponse( void );
	~buildResponse();

	// *		---------------- GETTERS ----------------
	const vector<char>	&getHttpResponse( void )const;

	const string 		&getStatusLine( void )const;
	const string 		&getTimeStamp( void )const;
	const string 		&getContentType( void )const;
	const string 		&getTransfertEncoding( void )const;
	const string 		&getContentLenght( void )const;
	
	const string 		&getBody( void )const;
	
	const u_int32_t		&getBodyLenght( void )const;
	
	// *		---------------- SETTERS ----------------
	void				setStatusLine( string );
	void				setTimeStamp( string );
	void				setContentType( string );
	void				setTransfertEncoding( string );
	void				setContentLenght( string );
	void				setBody( string );

	// *		---------------- PUB METHODS ----------------
	void				buildBody( const vector<char>& );
	void				buildHeaders( const e_errorCodes&, const string& );
	void				assembleResponse( void );
};

ostream& operator<<( ostream& output_stream, const buildResponse& input );
#pragma once

#include "utils_templates.hpp"
#include "libraries.hpp"
#include "errorCode.hpp"

#define HTTP_REPONSE_SEPARATOR "\r\n"
#define HTTP_PROTOCOL "HTTP/1.1"
#define SPACE 0x20

class buildResponse
{

private:

	// Useless Coplian methods
	buildResponse( const buildResponse& copy );
	buildResponse& operator=( const buildResponse& right_operator );

	// Methods
	stringstream buildContentType( string )const;
	string extractType( string& )const;

	map<string, string> _mimeTypes;

	// Output Final Answer
	vector<char> _httpResponse;
	
	// Headers
	stringstream _masterHeader; // TODO : getter, setter, init list and <<

	stringstream _statusLine;
	stringstream _timeStamp; // TODO : need to code a timestamp generator
	stringstream _contentType;

	stringstream _transfertEncoding; // ! OPTION ONE
	//             OR
	stringstream _contentLenght; // ! OPTION TWO

	// Body
	string _body;

	// Utils
	u_int32_t _bodyLenght; // TODO : getter, setter, init list and <<

	// TODO : Create variables for coockies

protected:

	// Some protected members

public:

	buildResponse( void );
	~buildResponse();

	// *		---------------- GETTERS ----------------
	const vector<char> &getHttpResponse( void )const;

	const stringstream &getStatusLine( void )const;
	const stringstream &getTimeStamp( void )const;
	const stringstream &getContentType( void )const;
	const stringstream &getTransfertEncoding( void )const;
	const stringstream &getContentLenght( void )const;
	
	const string &getBody( void )const;
	
	// const string &get( void )const;
	
	// *		---------------- SETTERS ----------------
	void	setStatusLine( string );
	void	setTimeStamp( string );
	void	setContentType( string );
	void	setTransfertEncoding( string );
	void	setContentLenght( string );

	void	setBody( string );

	// *		---------------- METHODS ----------------
	void buildBody( const vector<char>& );
	void buildHeaders( const e_errorCodes&, const string& );
	void assembleResponse( void );


};

ostream& operator<<( ostream& output_stream, const buildResponse& input );
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

	// TODO : clean the class
	// Useless Coplian methods
	buildResponse( const buildResponse& copy );
	buildResponse& operator=( const buildResponse& right_operator );

	// Methods
	string buildContentType( const string& )const;
	string extractType( const string& )const;

	map<string, string> _mimeTypes;

	// Output Final Answer
	vector<char> _httpResponse;
	
	// Headers
	string _masterHeader; // TODO : getter, setter, init list and <<

	string _statusLine;
	string _timeStamp; // TODO : need to code a timestamp generator
	string _contentType;

	string _transfertEncoding; // ! OPTION ONE
	//             OR
	string _contentLenght; // ! OPTION TWO

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

	const string &getStatusLine( void )const;
	const string &getTimeStamp( void )const;
	const string &getContentType( void )const;
	const string &getTransfertEncoding( void )const;
	const string &getContentLenght( void )const;
	
	const string &getBody( void )const;
	
	const string &get( void )const;
	
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
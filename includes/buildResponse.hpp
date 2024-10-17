#pragma once

#include "utils_templates.hpp"
#include "libraries.hpp"

class buildReponse
{

private:

	// Useless Coplian methods
	buildReponse( const buildReponse& copy );
	buildReponse& operator=( const buildReponse& right_operator );


	// Output Final Answer
	vector<char> _httpResponse;
	
	// Headers
	string _statusLine;
	string _timeStamp; // TODO : need to code a timestamp generator
	string _contentType;

	string _transfertEncoding; // ! OPTION ONE
	//             OR
	string _contentLenght; // ! OPTION TWO

	// Body
	string _body;

	// TODO : Create variables for coockies

protected:

	// Some protected members

public:

	buildReponse( void );
	~buildReponse();

	// *		---------------- GETTERS ----------------
	// ! STATUS LINE
	const vector<char> &getHttpResponse( void )const;

	// ! HEADERS
	const string &getStatusLine( void )const;
	const string &getTimeStamp( void )const;
	const string &getContentType( void )const;
	const string &getTransfertEncoding( void )const;
	const string &getContentLenght( void )const;
	
	// ! BODY
	const string &getBody( void )const;
	
	// ! Template getter
	// const string &get( void )const;
	
	// *		---------------- SETTERS ----------------
	// ! STATUS LINE
	void	setHttpResponse( vector<char> input /* could be string or char*   */ );

	// ! HEADERS
	void	setStatusLine( string input );
	void	setTimeStamp( string input );
	void	setContentType( string input );
	void	setTransfertEncoding( string input );
	void	setContentLenght( string input );

	// ! BODY
	void	setBody( string input );
};

ostream& operator<<( ostream& output_stream, const buildReponse& input );


// !!!!!! ---------- FUNCTIONS DECLARATIONS ---------------


buildReponse::buildReponse( void ){

	// TODO : List init some values ?
}


buildReponse::buildReponse( const buildReponse& copy ) :
	_httpResponse(copy._httpResponse),
	_statusLine(copy._statusLine),
	_timeStamp(copy._timeStamp),
	_contentType(copy._contentType),
	_transfertEncoding(copy._transfertEncoding),
	_contentLenght(copy._contentLenght),
	_body(copy._body){}


buildReponse& buildReponse::operator=( const buildReponse& right_operator ){

	if (this != &right_operator){

		// Reassign every value with the getter value 
		this->_httpResponse = right_operator.getHttpResponse();
		
		this->_statusLine = right_operator.getStatusLine();
		this->_timeStamp = right_operator.getTimeStamp();
		this->_contentType = right_operator.getContentType();
		this->_transfertEncoding = right_operator.getTransfertEncoding();
		this->_contentLenght = right_operator.getContentLenght();
		this->_body = right_operator.getBody();
	}
	return *this;
}

// ! DESTRUCTOR
buildReponse::~buildReponse( void ){

	// TODO : clean the httpResponse on each run ??
	_httpResponse.clear();

	_statusLine.clear();
	_timeStamp.clear();
	_contentType.clear();
	_transfertEncoding.clear();
	_contentLenght.clear();
	_body.clear();
}

// *	------------------- GETTERS ----------------------------------
const vector<char>& buildReponse::getHttpResponse( void )const{ return _httpResponse; }

const string &buildReponse::getStatusLine( void )const{ return _statusLine; }
const string &buildReponse::getTimeStamp( void )const{ return _timeStamp; }
const string &buildReponse::getContentType( void )const{ return _contentType; }
const string &buildReponse::getTransfertEncoding( void )const{ return _transfertEncoding; }
const string &buildReponse::getContentLenght( void )const{ return _contentLenght; }
const string &buildReponse::getBody( void )const{ return _body; }


// *	------------------- SETTERS ----------------------------------

// ! could be string or char* as an input
void buildReponse::setHttpResponse( vector<char> input){

	// TODO : concatenate to the vector on differents ways depending if it's a string, a vector or a `char*`
}

void	buildReponse::setStatusLine( string input ){ _statusLine = input; }
void	buildReponse::setTimeStamp( string input ){ _timeStamp = input; }
void	buildReponse::setContentType( string input ){ _contentType = input; }
void	buildReponse::setTransfertEncoding( string input ){ _transfertEncoding = input; }
void	buildReponse::setContentLenght( string input ){ _contentLenght = input; }
void	buildReponse::setBody( string input ){ _body = input; }



ostream& operator<<( ostream& output_stream, const buildReponse& right_input ){
	
	output_stream << BOLD_HIGH_INTENSITY_GREEN << "Status Line = \n";
	output_stream << right_input.getStatusLine();

	output_stream << BOLD_BLUE << "\n\nTimeStamp = \n";
	output_stream << right_input.getTimeStamp();

	output_stream << BOLD_BLUE << "\n\nContent-Type = \n";
	output_stream << right_input.getContentType();

	output_stream << BOLD_BLUE << "\n\nTransfert-Encoding = \n";
	output_stream << right_input.getTransfertEncoding();

	output_stream << BOLD_BLUE << "\n\nContent-Lenght = \n";
	output_stream << right_input.getContentLenght();

	output_stream << BOLD_HIGH_INTENSITY_RED << "\n\nHTTP BODY = \n";
	output_stream << right_input.getBody();

	return output_stream;
}

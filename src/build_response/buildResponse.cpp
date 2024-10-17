#include "buildResponse.hpp"


buildResponse::buildResponse( void ){

	// TODO : List init some values ?
}


buildResponse::buildResponse( const buildResponse& copy ) :
	_httpResponse(copy._httpResponse),
	_statusLine(copy._statusLine),
	_timeStamp(copy._timeStamp),
	_contentType(copy._contentType),
	_transfertEncoding(copy._transfertEncoding),
	_contentLenght(copy._contentLenght),
	_body(copy._body){}


buildResponse& buildResponse::operator=( const buildResponse& right_operator ){
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
buildResponse::~buildResponse( void ){

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
const vector<char>& buildResponse::getHttpResponse( void )const{ return _httpResponse; }
const string &buildResponse::getStatusLine( void )const{ return _statusLine; }
const string &buildResponse::getTimeStamp( void )const{ return _timeStamp; }
const string &buildResponse::getContentType( void )const{ return _contentType; }
const string &buildResponse::getTransfertEncoding( void )const{ return _transfertEncoding; }
const string &buildResponse::getContentLenght( void )const{ return _contentLenght; }
const string &buildResponse::getBody( void )const{ return _body; }


// *	------------------- SETTERS ----------------------------------

// ! could be string or char* as an input
void buildResponse::setHttpResponse( vector<char> input){

	// TODO : concatenate to the vector on differents ways depending if it's a string, a vector or a `char*`
}

void	buildResponse::setStatusLine( string input ){ _statusLine = input; }
void	buildResponse::setTimeStamp( string input ){ _timeStamp = input; }
void	buildResponse::setContentType( string input ){ _contentType = input; }
void	buildResponse::setTransfertEncoding( string input ){ _transfertEncoding = input; }
void	buildResponse::setContentLenght( string input ){ _contentLenght = input; }
void	buildResponse::setBody( string input ){ _body = input; }



ostream& operator<<( ostream& output_stream, const buildResponse& right_input ){
	
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

void buildResponse::buildBody( vector<char>& bodyInput){

	// Extract the vector towards a string.
	string tempBody = std::string( bodyInput.begin(), bodyInput.end());
	
	_body += HTTP_HEADER_BODY_BOUNDARY;
	_body += tempBody;

	// Calculate size of body for headers. (!= from lenght() )
	_bodyLenght = tempBody.size();
}